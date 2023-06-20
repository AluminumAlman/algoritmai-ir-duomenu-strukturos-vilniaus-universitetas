#ifndef BITMAP_H_
#define BITMAP_H_ 1

#include <stdlib.h>

#define GUARD_BYTES 8

typedef struct BitMap BitMap;

extern BitMap BitMap_new(size_t size);
extern int BitMap_isSetAt(const BitMap *map, const size_t index);
extern int BitMap_setAt(BitMap *map, const size_t index);
extern int BitMap_unsetAt(BitMap *map, const size_t index);
extern void BitMap_destroy(BitMap *map);

struct BitMap {
        size_t size;
        unsigned char *map;
};

BitMap BitMap_new(size_t size)
{
        BitMap result = {0};
        result.map = malloc(size + GUARD_BYTES);
        if(result.map == NULL) return result;
        result.size = size;
        return result;
}

int BitMap_isSetAt(const BitMap *map, const size_t index)
{
        if((index >> 3) > map->size) return -1;
        return (map->map[index >> 3]) & (1 << (index & 7));
}

int BitMap_setAt(BitMap *map, const size_t index)
{
        if((index >> 3) > map->size) return -1;
        map->map[index >> 3] |= (1 << (index & 7));
        return 0;
}

int BitMap_unsetAt(BitMap *map, const size_t index)
{
        if((index >> 3) > map->size) return -1;
        map->map[index >> 3] &= ~(1 << (index & 7));
        return 0;
}

void BitMap_destroy(BitMap *map)
{
        if(map->map != NULL) free(map->map);
        map->map = NULL;

        map->size = 0;
        return;
}

#undef GUARD_BYTES

#endif // #ifndef BITMAP_H_
