#ifndef MYOS__SYSTEM_H
#define MYOS__SYSTEM_H

#include <common/types.h>
#include <multiboot.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <gui/console.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/rtc.h>
#include <drivers/pit.h>
#include <gui/canvas.h>
#include <gui/shell.h>
#include <common/convert.h>
#include <common/memfunc.h>
#include <multitasking.h>
#include <gui/shell.h>

namespace myos
{
    class System
    {
    public:
        static GlobalDescriptorTable* GDT;
        static MemoryManager* memManager;
        static TaskManager* taskManager;
        static hardwarecommunication::InterruptManager* interruptsManager;
        static drivers::DriverManager* driverManager;
        static hardwarecommunication::PeripheralComponentInterconnectController* pci;
        static drivers::PIT* pit;
        static drivers::RTC* rtc;
        static gui::Shell* shell;
        static gui::Canvas* canvas;
    };
}

#endif