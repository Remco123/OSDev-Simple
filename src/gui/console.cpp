#include <gui/console.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::gui;

Canvas* Console::screen = 0;
Font* Console::font = 0;
ConsoleKeyboardEventHandler* Console::keyboard = 0;
int Console::YOffset = 0; 
int Console::XOffset = 0;

void Console::Init(Canvas* cnvas, Font* fnt, ConsoleKeyboardEventHandler* kbhandler)
{
    Console::screen = cnvas;
    Console::font = fnt;
    Console::keyboard = kbhandler;
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
        keyboard->ClearBuffer();

        for(uint8_t i = 0; i < MemoryFunctions::strlen(intput); i++) //Loop though data
        {
            char c = intput[i];
            switch(c)
            {
                case '\n':
                    if(numChars > 0)
                    {
                        YOffset += 10;
                        XOffset = 0;
                        return result;
                    }
                case '\b':
                    {
                        if(numChars > 0)
                        {
                            AddMessage(Convert::CharToStr('\b'));
                            result[numChars] = ' ';
                            numChars--;
                        }
                    }
                    break;
                default: 
                    {
                        result[numChars] = c;
                        numChars++; 

                        AddMessage(Convert::CharToStr(c));
                    }
                    break;
            }
        }
    }
}
void Console::AddBootMessage(char* msg,...)
{
    
}

void Console::AddMessage(char* msg,...)
{
    for(uint8_t i = 0; i < MemoryFunctions::strlen(msg); i++) //Loop though data
    {
        char c = msg[i];
        switch(c)
        {
            case '\n':
                YOffset+=10;
                XOffset = 0;
            case '\b':
            {
                XOffset -= 8;
                uint8_t* w_and_h = font->GetCharData(' ', 10);
                screen->DrawFillRect(Color::Create(0,200,0), XOffset, YOffset, w_and_h[0],w_and_h[1]);
                break;
            }
            default: 
            {
                //Draw Char on screen;
                font->DrawTo(screen, Convert::CharToStr(c), 0, XOffset, YOffset, Color::Create(0,0,0));
                XOffset += 8;
                break;
            }
        }
    }
}
void Console::ProccesMessage(char* msg)
{

}