#include <gui/console.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::gui;

Console::Console(Canvas* canv, Font* fnt, ConsoleKeyboardEventHandler* kb)
{
    this->screen = canv;
    this->font = fnt;
    this->keyboard = kb; 
}

void Console::Write(char* str,...)
{    
    for(int i = 0; str[i] != '\0'; ++i) //Loop though string
    {
        char chr = str[i];
        switch(chr)
        {
            case '\n':
                YOffset += 10; //Add height of space
                XOffset = 0; //Reset x
                CheckForScroll();
                break;
            default:
                font->DrawTo(screen, Convert::CharToStr(chr), 10, XOffset, YOffset, Color::Create(0,0,0));
                XOffset+= this->font->GetCharData(chr,10)[0];
                if(XOffset + 10 > screen->Width)
                {
                    YOffset += 10;
                    XOffset = 0;
                    CheckForScroll();
                }
        }
    }
    CheckForScroll();
}
void Console::WriteLine(char* msg,...)
{
    Write(msg);
    Write("\n");
}

char* Console::ReadLine()
{
    bool InputDone = false;
    char* result = (char*)MemoryManager::activeMemoryManager->malloc(255);
    MemoryFunctions::memset(result, 0, 255);
    uint8_t numChars = 0;
    while(!InputDone)
    {
        while(!keyboard->DataAvailible); //Wait for data
        char* intput = keyboard->GetBuffer();

        for(uint8_t i = 0; i < MemoryFunctions::strlen(intput); i++) //Loop though data
        {
            char c = intput[i];
            switch(c)
            {
                case '\n':
                #if 0 //Set to 1 to check for empty string
                    if(numChars > 0)
                    {
                        YOffset += 10;
                        XOffset = 0;
                        return result;
                    }
                #else
                    YOffset += 10;
                    XOffset = 0;
                    return result;
                #endif
                case '\b':
                    {
                        if(numChars > 0)
                        {
                            uint16_t CharWidth = this->font->GetCharData(result[numChars - 1],10)[0];
                            uint16_t CharHeight = this->font->GetCharData(result[numChars - 1],10)[1];
                            screen->DrawFillRect(Color::Create(200,200,200), XOffset - CharWidth, YOffset, CharWidth, CharHeight);
                            
                            result[numChars] = ' ';
                            numChars--;
                            XOffset -= CharWidth;
                        }
                    }
                    break;
                default: 
                    {
                        result[numChars] = c;
                        numChars++; 

                        Write(Convert::CharToStr(c));
                    }
                    break;
            }
        }
        keyboard->ClearBuffer();
    }
}
void Console::CheckForScroll()
{  
    if(this->screen != 0)
    {
        if(YOffset > screen->Height - 20)
        {
            ScrollDown();
            YOffset -= 10;
        }
    }
        
}
void Console::ScrollDown()
{
    uint32_t color = Color::Create(200, 200, 200).GetARGB();
    uint32_t* _buffer = (uint32_t*)screen->GetFramebufferAddr();
    for (int i = 0; i < screen->Height; i++)
    {
        for (int m = 0; m < screen->Width; m++)
        {
            _buffer[i * screen->Width + m] = _buffer[(i + 10) * screen->Width + m];
        }
    }

    for (int i = screen->Height - 10; i < screen->Height; i++)
    {
        for (int m = 0; m < screen->Width; m++)
        {
            _buffer[i * screen->Width + m] = color;

        }
    }
}