#ifndef __MYOS__COMMON__MATH_H
#define __MYOS__COMMON__MATH_H

#include <common/types.h>

namespace myos
{
    namespace common
    {
        class Math
        {
        public:
            static int Abs(int v)
            {
                if(v == 0)
                    return 0;
                if(v < 0)
                    return -v;
                if(v > 0)
                    return v;
                return v;
            }
            static int Sign(int v)
            {
                if(v < 0)
                    return -1;
                if(v == 0)
                    return 0;
                if(v > 1)
                    return 1;
                return 0;
            }
        };
    }
}

#endif