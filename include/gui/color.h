#ifndef __MYOS__GUI__COLOR_H
#define __MYOS__GUI__COLOR_H

#include <common/types.h>

namespace myos
{
    namespace gui
    {
        class Color
        {
        public:

            bool operator==(const Color& b) const
            {
                return (this->A == b.A) && (this->B == b.B) && (this->G == b.G) && (this->R == b.R);
            }

            common::uint32_t R;
            common::uint32_t G;
            common::uint32_t B;
            common::uint32_t A;

            static Color Create(common::uint32_t R, common::uint32_t G, common::uint32_t B);
            static Color Create(common::uint32_t R, common::uint32_t G, common::uint32_t B, common::uint32_t A);
            float GetBrightness();
            float GetHue();
            float GetSaturation();
        };
    }
}

#endif