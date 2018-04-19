#ifndef __MYOS__GUI__FONTS_FONT_H
#define __MYOS__GUI__FONTS_FONT_H

#include <common/types.h>
#include <gui/canvas.h>

namespace myos
{
    namespace gui
    {
        class Font
        {
        public:
            Font(char* name);
            char* Name;

            virtual common::uint8_t* GetSupportedSizes();
            virtual common::uint8_t* GetCharData(char c, common::uint8_t fontsize);
            virtual void DrawTo(myos::gui::Canvas* canvas, char* str, common::uint8_t fontSize, common::uint32_t x, common::uint32_t y, gui::Color color);
        };
    }
}

#endif