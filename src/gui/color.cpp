#include <gui/color.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;

Color Color::Create(uint32_t R, uint32_t G, uint32_t B)
{
    Color col;
    col.R = R;
    col.G = G;
    col.B = B;
    col.A = 255;
    return col;
}
Color Color::Create(uint32_t R, uint32_t G, uint32_t B, uint32_t A)
{
    Color col;
    col.R = R;
    col.G = G;
    col.B = B;
    col.A = A;
    return col;
}
float Color::GetBrightness()
{
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;

    float max, min;

    max = r; min = r;

    if (g > max) max = g;
    if (b > max) max = b;

    if (g < min) min = g;
    if (b < min) min = b;

    return (max + min) / 2;
}
float Color::GetHue()
{
    if (R == G && G == B)
        return 0; // 0 makes as good an UNDEFINED value as any

    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;

    float max, min;
    float delta;
    float hue = 0.0f;

    max = r; min = r;

    if (g > max) max = g;
    if (b > max) max = b;

    if (g < min) min = g;
    if (b < min) min = b;

    delta = max - min;

    if (r == max)
    {
        hue = (g - b) / delta;
    }
    else if (g == max)
    {
        hue = 2 + (b - r) / delta;
    }
    else if (b == max)
    {
        hue = 4 + (r - g) / delta;
    }
    hue *= 60;

    if (hue < 0.0f)
    {
        hue += 360.0f;
    }
    return hue;
}
float Color::GetSaturation()
{
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;

    float s = 0;

    float max = r;
    float min = r;

    if (g > max) max = g;
    if (b > max) max = b;

    if (g < min) min = g;
    if (b < min) min = b;

    // if max == min, then there is no color and
    // the saturation is zero.
    //
    if (max != min)
    {
        float l = (max + min) / 2;

        if (l <= .5)
        {
            s = (max - min) / (max + min);
        }
        else
        {
            s = (max - min) / (2 - max - min);
        }
    }
    return s;
}   

uint32_t Color::GetARGB()
{
    return 0xFF000000 |   //Opacity
           R << 16 | //B
           G << 8  | //G
           B;        //R
}