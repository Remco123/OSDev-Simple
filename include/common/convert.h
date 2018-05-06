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
                if(i == 0)
                    return "0";
                static char output[24];  // 64-bit MAX_INT is 20 digits
                char* p = &output[23];

                for(*p--=0;i;i/=10) *p--=i%10+0x30; 
                return ++p;    
            }
            static void itoa(char *buf, unsigned long int n, int base)
            {
                unsigned long int tmp;
                int i, j;

                tmp = n;
                i = 0;

                do {
                    tmp = n % base;
                    buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
                } while (n /= base);
                buf[i--] = 0;

                for (j = 0; j < i; j++, i--) {
                    tmp = buf[j];
                    buf[j] = buf[i];
                    buf[i] = tmp;
                }
            }
            static int isspace(char c) {
                return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
            }  
            static int atoi(char * string) {
                int result = 0;
                unsigned int digit;
                int sign;

                while (isspace(*string)) {
                    string += 1;
                }

                /*
                * Check for a sign.
                */

                if (*string == '-') {
                    sign = 1;
                    string += 1;
                } else {
                    sign = 0;
                    if (*string == '+') {
                        string += 1;
                    }
                }

                for ( ; ; string += 1) {
                    digit = *string - '0';
                    if (digit > 9) {
                        break;
                    }
                    result = (10*result) + digit;
                }

                if (sign) {
                    return -result;
                }
                return result;
            }
            static char* CharToStr(char c)
            {
                char* result = " ";
                result[0] = c;
                return result;
            }
        };
    }
}

#endif