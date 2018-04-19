#include <gui/image.h>

using namespace myos::common;
using namespace myos;
using namespace myos::gui;

Image::Image(uint32_t width, uint32_t height)
{
    this->Widht = width;
    this->Height = height;

    for(int i = 0; i < width*height; i++)
        ImageData[i] = 0; //Clear Data
}
Image::~Image()
{
    MemoryManager::activeMemoryManager->free(this->ImageData);
}
Color Image::GetPixel(uint32_t x, uint32_t y)
{
    uint32_t position = x*Widht + y;
    return Color::Create(ImageData[position], ImageData[position++], ImageData[position++], ImageData[position++]);
}
void Image::SetPixel(uint32_t x, uint32_t y, Color color)
{
    uint32_t position = x*Widht + y;
    ImageData[position] = color.R;
    ImageData[position++] = color.G;
    ImageData[position++] = color.B;
    ImageData[position++] = color.A;
}
void Image::Resize(uint32_t newWidth, uint32_t newHeight, ResizeMode mode)
{

}
void Image::DrawTo(Canvas* canvas, uint32_t xPos, uint32_t yPos, uint32_t width, uint32_t heigt)
{
    //Ignore width and height for now

    for(uint32_t x = 0; x < this->Widht; x++)
    {
        for(int32_t y = 0; y < this->Height; y++)
        {
            canvas->SetPixel(x + xPos, y + yPos, GetPixel(x, y));
        }
    }
}
void Image::Load(uint8_t* data)
{
    //Specific for every image
}