#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))


#include <common/types.h>
#include <multiboot.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/rtc.h>
#include <drivers/serial.h>
#include <gui/canvas.h>
#include <gui/color.h>
#include <gui/font.h>
#include <gui/image.h>
#include <gui/bmp.h>
#include <gui/arial.h>

#include <gui/testimage.h>
#include <common/convert.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;


void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}

void printfHexG(Canvas* canv,uint8_t key)
{
    static uint8_t x=20;

    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    
    Arial ari;
    ari.DrawTo(canv, foo, 10, x, 5, Color::Create(255, 255, 255));
    x+= 20;
}

void printfHex32G(Canvas* canv, uint32_t key)
{
    printfHexG(canv,(key >> 24) & 0xFF);
    printfHexG(canv,(key >> 16) & 0xFF);
    printfHexG(canv,(key >> 8) & 0xFF);
    printfHexG(canv, key & 0xFF);
}


class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t multiboot_magic)
{
    //uint32_t* pixel = (uint32_t*)(fb + mbi->framebuffer_pitch * y + 4 * x);

    //printf("Starting Kernel...\n");
    printfHex32(multiboot_magic);

    multiboot_info_t* mbi = (multiboot_info_t*)multiboot_structure;

    printf("Starting gdt");
    GlobalDescriptorTable gdt;
    printf("     [DONE]\n");

    printf("Starting Memory Manager");
    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    printf(" Heap: 0x");
    printfHex32(heap);
    
    void* allocated = memoryManager.malloc(1024);
    printf(" Allocated: 0x");
    printfHex32((uint32_t)allocated);;
    printf("     [DONE]\n");

    printf("Starting Interrupts Controller");
    InterruptManager interrupts(0x20, &gdt);
    printf("     [DONE]\n");
    
    printf("Starting Driver Controller");
    DriverManager drvManager;
    printf("     [DONE]\n");
    
    printf("Loading keyboard ");
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);
    printf("     [DONE]\n");
    
    printf("Loading mouse ");
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    drvManager.AddDriver(&mouse);
    printf("     [DONE]\n");
        
    printf("Starting PCI");
    PeripheralComponentInterconnectController PCIController;
    printf("     [DONE]\n");
    printf("Loading additional drivers for devices:\n$----------------------------------------$\n");
    PCIController.SelectDrivers(&drvManager, &interrupts);
    printf("$----------------------------------------$\n");
        
    printf("Activating Drivers");
    drvManager.ActivateAll();
    printf("     [DONE]\n");
        
    BMPImage bitmap(0, 0);
    bitmap.Load(image_bmp);
    Canvas canvas((void*)mbi->framebuffer_addr, mbi->framebuffer_pitch, (uint32_t)mbi->framebuffer_width, (uint32_t)mbi->framebuffer_height, (uint8_t)mbi->framebuffer_bpp);
    bitmap.DrawTo(&canvas, 50, 50, 300, 300);

    printfHex32G(&canvas, mbi->framebuffer_addr);


    printf("Activating Interrupts");
    interrupts.Activate();
    printf("     [DONE]\n");

    //bitmap.DrawTo(gfx, 0, 0, 300, 300);

    /*
    while(1); //Disable fonts

    int lastsec = -1;
    while(1)
    {
        int sec = rtc.GetSecond();
        if(sec != lastsec)
        {
            gfx->DrawFillRect(Color::Create(0, 0, 0), 40, 40, 50, 20);
            arial.DrawTo(gfx, Convert::itoa(sec), 10, 50, 50, Color::Create(255, 255, 255));
            lastsec = sec;
        }
    }
    */

    for(;;) {
        asm("hlt");
    }
}
