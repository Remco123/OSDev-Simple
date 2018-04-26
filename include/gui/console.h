#ifndef MYOS__GUI__CONSOLE
#define MYOS__GUI__CONSOLE

#include <common/types.h>
#include <common/convert.h>
#include <gui/font.h>
#include <gui/canvas.h>
#include <drivers/keyboard.h>
#include <memorymanagement.h>
#include <common/memfunc.h>

void printf(char*,...);

namespace myos
{
    namespace gui
    {
        class ConsoleKeyboardEventHandler : public myos::drivers::KeyboardEventHandler
        {
        private:
            //What if there are more keys pressed before we handle them.
            char* buffer;
            common::uint8_t numData;
        public:
            bool DataAvailible;
            ConsoleKeyboardEventHandler()
            {
                numData = 0;
                buffer = (char*)MemoryManager::activeMemoryManager->malloc(255);
            }

            void OnKeyDown(char c)
            {
                buffer[numData] = c;
                numData++;

                DataAvailible = true;
            }

            void ClearBuffer()
            {
                myos::common::MemoryFunctions::memset(buffer, 0, numData);
                numData = 0;
                DataAvailible = false;
            }

            char* GetBuffer()
            {
                numData = 0;
                DataAvailible = false;
                return buffer;
            }
        };


        ////------- Console Class -------///// Warning: This class is always static
        class Console
        {
        private:
            Canvas* screen;
            Font* font;
            static Console* instance;
        public:
            static void Write(char* msg,...);
            static void WriteLine(char* msg,...);
            static char* ReadLine();

            void Write(char* msg,...);
            void WriteLine(char* msg,...);
            char* ReadLine();
        };
    }
}

#endif