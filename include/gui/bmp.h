#ifndef __MYOS__GUI__BMP_H
#define __MYOS__GUI__BMP_H

#include <memorymanagement.h>
#include <common/types.h>
#include <gui/image.h>


namespace myos
{
    namespace gui
    {
        typedef struct tagBITMAPFILEHEADER {
            unsigned short bfType;
            unsigned int bfSize;
            unsigned short bfReserved1;
            unsigned short bfReserved2;
            unsigned int bfOffBits;
        }__attribute__((packed)) bmp_fileheader_t;

        typedef struct tagBITMAPINFOHEADER {
            unsigned int biSize;
            long biWidth;
            long biHeight;
            unsigned short biPlanes;
            unsigned short biBitCount;
            unsigned int biCompression;
            unsigned int biSizeImage;
            long biXPelsPerMeter;
            long biYPelsPerMeter;
            unsigned int biClrUsed;
            unsigned int biClrImportant;
        }bmp_infoheader_t;


        class BMPImage : public Image
        {
        public:
            BMPImage(common::uint32_t width, common::uint32_t height);
            
            void DrawTo(gui::Canvas* canvas, common::uint32_t x, common::uint32_t y, common::uint32_t width, common::uint32_t heigt);
            void Load(common::uint8_t* data);
        };
    }
}

#endif