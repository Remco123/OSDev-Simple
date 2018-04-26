#include <gui/font.h>

using namespace myos;
using namespace myos::gui;
using namespace myos::common;

void printfHex(uint8_t);

Font::Font(char* name)
{
    this->Name = name;
}

uint8_t* Font::GetSupportedSizes()
{
    static uint8_t arr[3];
 
    /* Some operations on arr[] */
    arr[0] = 10;
    
    return arr;
}
uint8_t *Font::GetCharData(char c, uint8_t fontsize)
{
    static uint8_t arr[225];

    arr[0] = 0;
    arr[1] = 0;

    return arr;
}

void Font::DrawTo(gui::Canvas* canvas, char* str, uint8_t fontSize, uint32_t x, uint32_t y, Color color)
{
    uint32_t Ypos = y;
    uint32_t Xpos = x;
    
    for(int i = 0; str[i] != '\0'; ++i) //Loop though string
    {
        char chr = str[i];
        switch(chr)
        {
            case '\n':
                Ypos += fontSize; //GetCharData(' ', fontSize)[1]; //Add height of space
                Xpos = x; //Reset x
                break;
            default:
                uint8_t* data = GetCharData(chr, fontSize);
                int Position = 2; //Skip Width and Height
                //Loop through char pixels

                for(uint32_t w = 0; w < data[0]; w++)
                {
                    for(uint32_t h = 0; h < data[1]; h++)
                    {
                        uint8_t value = data[Position];
                        if(value == 255)
                        {
                            canvas->SetPixel(Xpos + w, Ypos + h, color);
                        }
                        else if(value != 0) //Transparent Color
                        {
                            Color current = canvas->GetPixel(Xpos + w, Ypos + h);
                            float amount = value/(float)255;
                            uint8_t r = (uint8_t)((color.R * amount) + current.R * (1 - amount));
                            uint8_t g = (uint8_t)((color.G * amount) + current.G * (1 - amount));
                            uint8_t b = (uint8_t)((color.B * amount) + current.B * (1 - amount));

                            canvas->SetPixel(Xpos + w, Ypos + h, Color::Create(r, g, b));
                        }
                        Position++;
                    }
                }
                Xpos += data[0];
                break;
        }
    }
}