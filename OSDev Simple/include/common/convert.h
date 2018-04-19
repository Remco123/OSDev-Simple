#ifndef __MYOS__COMMON__CONVERT_H
#define __MYOS__COMMON__CONVERT_H

#include <common/types.h>

namespace myos
{
    namespace common
    {
        class Convert
        {
        public:
            static char* itoa(int i)
            {
                static char output[24];  // 64-bit MAX_INT is 20 digits
                char* p = &output[23];

                for(*p--=0;i;i/=10) *p--=i%10+0x30; 
                return ++p;    
            }
        };
    }
}

#endif