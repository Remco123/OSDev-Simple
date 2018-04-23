#ifndef __MYOS__MEMFUNC_H
#define __MYOS__MEMFUNC_H
 
#include <common/types.h>

namespace myos
{
    namespace common
    {
        class MemoryFunctions
        {
        public:
            static int memcmp(const void*, const void*, size_t);
            static void* memcpy(void*, const void*, size_t);
            static void* memmove(void*, const void*, size_t);
            static void* memset(void*, int, size_t);
            static size_t strlen(const char*);
        };
    }
}
 
#endif