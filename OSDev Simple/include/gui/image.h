#ifndef __MYOS__GUI__IMAGE_H
#define __MYOS__GUI__IMAGE_H

#include <memorymanagement.h>
#include <common/types.h>
#include <gui/color.h>
#include <gui/canvas.h>

namespace myos
{
    namespace gui
    {
        enum ResizeMode
        {
            NearestNeighbor, // 0.201676 Seconds
            Bilinear, // 1.079586 Seconds
            Bicubic // 3.162943 Seconds
        };

        class Image
        {
        protected:
            common::uint8_t* ImageData;
        public:
            Image(common::uint32_t width, common::uint32_t height);
            ~Image();

            common::uint32_t Widht;
            common::uint32_t Height;


            virtual gui::Color GetPixel(common::uint32_t x, common::uint32_t y);
            virtual void SetPixel(common::uint32_t x, common::uint32_t y, gui::Color color);
            virtual void Resize(common::uint32_t newWidth, common::uint32_t newHeight, ResizeMode mode = NearestNeighbor);
            virtual void DrawTo(gui::Canvas* canvas, common::uint32_t x, common::uint32_t y, common::uint32_t width, common::uint32_t heigt);
            virtual void Load(common::uint8_t* data);
        };
    }
}

#endif