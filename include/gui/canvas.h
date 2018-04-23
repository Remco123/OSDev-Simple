#ifndef __MYOS__GUI__CANVAS_H
#define __MYOS__GUI__CANVAS_H

#include <common/types.h>
#include <common/memfunc.h>
#include <common/math.h>
#include <drivers/driver.h>

#include <gui/color.h>

namespace myos
{
    namespace gui
    {
        class Canvas
        {
        private:
            void* framebuffer_addr;
            common::uint32_t pitch;
        public:
            common::uint32_t Width;
            common::uint32_t Height;
            common::uint8_t bpp;

            Canvas(void* framebuffer_addr, common::uint32_t pitch, common::int32_t width, common::uint32_t heigth, common::uint8_t bpp);
            ~Canvas();

            void SetPixel(common::uint32_t x, common::uint32_t y, gui::Color color);
            gui::Color GetPixel(common::uint32_t x, common::uint32_t y);

            //Common Graphics Functions
            void Clear();
            void Clear(gui::Color color);
            void DrawHorizontalLine(gui::Color color, int dx, int x1, int y1);
            void DrawVerticalLine(gui::Color color, int dx, int x1, int y1);
            void DrawLine(gui::Color color, int x1, int y1, int x2, int y2);
            void DrawDiagonalLine(gui::Color color, int dx, int dy, int x1, int y1);
            void DrawRect(gui::Color color, int x, int y, int width, int height);
            void DrawFillRect(gui::Color color, int x_start, int y_start, int width, int height);
            void DrawCircle(gui::Color color, int x_center, int y_center, int radius);
            void DrawFillCircle(gui::Color color, int x_center, int y_center, int radius);
            void DrawEllipse(gui::Color color, int x_center, int y_center, int x_radius, int y_radius);
            void FloodFill(int x, int y, gui::Color oldcolor, gui::Color newcolor);

            void* GetFramebufferAddr();
        };
    }
}

#endif