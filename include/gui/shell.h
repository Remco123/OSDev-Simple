#ifndef __MYOS__GUI__SHELL_H
#define __MYOS__GUI__SHELL_H

#include <common/types.h>
#include <gui/console.h>
#include <hardwarecommunication/cpu.h>
#include <hardwarecommunication/port.h>

namespace myos
{
    namespace gui
    {
        //TODO: Add blink cursor
        class Shell
        {
        private:
            gui::Console* console;
        public:
            Shell(gui::Console* cons);

            void Next();
        };  
    }
}

#endif