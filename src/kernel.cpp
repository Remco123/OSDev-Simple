#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

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
#include <drivers/vesa/vesaheaders.h>
#include <drivers/rtc.h>
#include <gui/canvas.h>
#include <gui/color.h>
#include <gui/font.h>
#include <gui/image.h>
#include <gui/bmp.h>
#include <gui/arial.h>
#include <gui/shell.h>
#include <common/convert.h>
#include <common/memfunc.h>
#include <multitasking.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

Canvas* currentCanvas;
Arial* currentFont;
Console* console;

void printf(char* str)
{
    if(console){
        console->Write(str);
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

void printfHex16(uint16_t key)
{
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}

void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}

class PrintKeyboardGraphicsHandler : public KeyboardEventHandler
{
private:
    Canvas* gfx; //Pointer to the canvas.
    Arial* font; //Pointer to the font.
    //ps: pointers are cool!
public:
    PrintKeyboardGraphicsHandler(Canvas* canv, Arial* font)
    {
        this->gfx = canv;
        this->font = font;
    }
    void OnKeyDown(char c)
    {
        static int x = 5;

        char* foo = " ";
        foo[0] = c;
        font->DrawTo(gfx, foo, 10, x, 10, Color::Create(255, 255, 255));
        x+=10;
    }
};

void PrintGraphicsInfo(multiboot_info_t* mbi)
{
    printf("//---------------- Graphics Info --------------------//\n");
    printf("Graphics Mode: "); printf(Convert::itoa(mbi->framebuffer_width)); printf("x"); printf(Convert::itoa(mbi->framebuffer_height)); printf("x"); printf(Convert::itoa(mbi->framebuffer_bpp)); printf("\n");

    VESA_INFO* info = (VESA_INFO*)mbi->vbe_control_info;
    if(info->signature[0] == 'V' && info->signature[3] == 'A') //We have info about the controller right?
    {
        printf("Mode Number: 0x"); printfHex16(mbi->vbe_mode); printf("\n");
        printf("Memory: "); printf(Convert::itoa(info->video_memory)); printf("\n");
        printf("Version: "); printfHex16(info->version); printf("\n");
        MODE_INFO* mode = (MODE_INFO*)mbi->vbe_mode_info;
        printf("PhysBase: "); printfHex32(mode->physbase); printf("\n");
    }
    printf("//---------------------------------------------------//\n");
}

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
    multiboot_info_t* mbi = (multiboot_info_t*)multiboot_structure;

    //Init the canvas for drawing to the screen.
    Canvas canvas((void*)mbi->framebuffer_addr, mbi->framebuffer_pitch, (uint32_t)mbi->framebuffer_width, (uint32_t)mbi->framebuffer_height, mbi->framebuffer_bpp);
    Arial arial;
    canvas.Clear(Color::Create(200, 200, 200));

    Console cons = Console(&canvas, &arial, 0);
    console = &cons;

    currentCanvas = &canvas;
    currentFont = &arial;

    PrintGraphicsInfo(mbi);

    printf("Starting gdt\n");
    GlobalDescriptorTable gdt;

    printf("Loading CPU Info\n");
    CPU::PrintInfo();

    printf("Starting Memory Manager\n");
    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    printf("Starting Task Manager\n");
    TaskManager taskManager;
    
    printf("Starting Interrupts Controller\n");
    InterruptManager interrupts(0x20, &gdt, &taskManager);

    printf("Starting Driver Controller\n");
    DriverManager drvManager;
    
    printf("Loading keyboard\n");
    ConsoleKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);

    cons.keyboard = &kbhandler;
    
    printf("Loading mouse\n");
    MouseEventHandler mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    drvManager.AddDriver(&mouse);
        
    printf("Starting PCI\n");
    PeripheralComponentInterconnectController PCIController;
    printf("Loading additional drivers for devices:\n"); printf("$----------------------------------------$\n");
    PCIController.SelectDrivers(&drvManager, &interrupts);
    printf("$----------------------------------------$\n");
        
    printf("Activating Drivers\n");
    drvManager.ActivateAll();
        
    printf("Activating Interrupts\n");
    interrupts.Activate();

    canvas.DrawFillRect(Color::Create(48, 186, 117), canvas.Width - 85, 0, 85,30);
    arial.DrawTo(&canvas, "Remco OS\nVersie: 0.12", 10, canvas.Width - 80, 5, Color::Create(0,0,0));

    Shell shell(&cons);

    while(1)
    {
        shell.Next();
    }


    while(1);
}
