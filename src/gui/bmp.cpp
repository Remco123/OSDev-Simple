#include <gui/bmp.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;

void printf(char*);
void printfHex(uint8_t);

BMPImage::BMPImage(uint32_t width, uint32_t height)
: Image(width, height){

}

int Invert(int y, int height)
{
   int row = 0;
   if (height > 0)
   {
       row = (height - y - 1);
   }
   else
   {
       row = y;
   }
   return row;
}

void BMPImage::DrawTo(gui::Canvas* canvas, common::uint32_t xpos, common::uint32_t ypos, common::uint32_t width, common::uint32_t heigt)
{
    int alignment = 0;

    alignment = (this->Widht * 3) % 4;
    if (alignment != 0)
    {
        alignment = 4 - alignment;
    }  

    int offset, rowOffset, arrayOffset;

    for (int y = 0; y < this->Height; y++)
    {
        rowOffset = (this->Height - y - 1) * (this->Widht * 3 + alignment);

        for (int x = 0; x < this->Widht; x++)
        {
            offset = rowOffset + x * 3;
            
            uint32_t b = ImageData[offset + 0];
            uint32_t g = ImageData[offset + 1];
            uint32_t r = ImageData[offset + 2];

            canvas->SetPixel(x + xpos, y + ypos, Color::Create(r, g, b));
        }
    }  
}

void BMPImage::Load(uint8_t* data)
{
    bmp_fileheader_t* h = (bmp_fileheader_t*)data;
    unsigned int offset = h->bfOffBits;

    bmp_infoheader_t* info = (bmp_infoheader_t*)( data + sizeof(bmp_fileheader_t));

    this->Widht = info->biWidth;
    this->Height = info->biHeight;
    
    this->ImageData = (uint8_t*)((unsigned int)data + offset);

    /*
    printf("Width: "); printfHex(info->biWidth);
    printf(" Height: "); printfHex(info->biHeight);
    printf(" BPP: "); printfHex(info->biBitCount);
    */
}