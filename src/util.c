#include <pokeagb/pokeagb.h>

void CpuFastSet(void* src, void* dst, u32 mode)
{
    __asm__("swi 0xC");
}

u16 rand_range(u16 min, u16 max)
{
    if (min == max) return min;
    return (rand() % (max - min)) + min;
}
