#ifndef MYOS__GUI__CONSOLE
#define MYOS__GUI__CONSOLE

#include <common/types.h>
#include <common/convert.h>
#include <gui/font.h>
#include <gui/canvas.h>
#include <drivers/keyboard.h>
#include <memorymanagement.h>
#include <common/memfunc.h>

namespace myos
{
    namespace gui
    {
        class ConsoleKeyboardEventHandler : public myos::drivers::KeyboardEventHandler
        {
        private:
            char buffer;
        public:
            bool DataAvailible;
            ConsoleKeyboardEventHandler()
            { }

            void OnKeyDown(char c)
            {
                buffer = c;
                DataAvailible = true;
            }

            void ClearBuffer()
            {
                DataAvailible = false;
            }

            char* GetBuffer()
            {
                DataAvailible = false;
                return myos::common::Convert::CharToStr(buffer);
            }
        };


        ////------- Console Class -------///// Warning: This class is always static
        class Console
        {
        private:
            Font* font;

            int YOffset = 0;
            int XOffset = 0;
        public:
            Canvas* screen;
            ConsoleKeyboardEventHandler* keyboard;
            Console(Canvas* canv, Font* fnt, ConsoleKeyboardEventHandler* kb);

            void Write(char* msg,...);
            void WriteLine(char* msg,...);
            char* ReadLine();

            void CheckForScroll();
            void ScrollDown();
            void Clear();
        };
    }
}

#endif