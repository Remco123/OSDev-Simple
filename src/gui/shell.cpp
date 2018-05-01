#include <gui/shell.h>


using namespace myos;
using namespace myos::common;
using namespace myos::gui;
using namespace myos::hardwarecommunication;

void printf(char*);

void reboot()
{
    uint8_t good = 0x02;
    while (good & 0x02)
        good = Port8Bit::Read8(0x64);
    Port8Bit::Write8(0x64, 0xFE);
    printf("Power off pc if you see this message.");
loop:
    asm volatile ("hlt"); /* if that didn't work, halt the CPU */
    goto loop; /* if a NMI is received, halt again */
}

int strcmp(const char *s1, const char *s2)
{
    while ((*s1 == *s2) && *s1) { ++s1; ++s2; }
    return (((int) (unsigned char) *s1) - ((int) (unsigned char) *s2) == 0) ? 1 : 0;
}

//#############################################################





//##############################################################
Shell::Shell(gui::Console* cons)
{
    this->console = cons;  
}

void Shell::Next()
{
    this->console->Write("==> ");
    char* input = this->console->ReadLine();
    
    if(strcmp(input,"cpu") == 1)
        hardwarecommunication::CPU::PrintInfo();
    else if(strcmp(input,"hallo") == 1)
        console->WriteLine("Jumbo");
    else if(strcmp(input, "reboot") == 1)
        reboot();
    else if(strcmp(input, "memtest") == 1)
    {
        if(!CPU::SSE)
        {
            console->WriteLine("Need SSE Support for memtest");
        }
        else
        {
            console->WriteLine("Test Without SSE");
            CPU::SSE = false;

            void* from = MemoryManager::activeMemoryManager->malloc(1024);
            void* to = MemoryManager::activeMemoryManager->malloc(1024);

            for(int i = 0; i < 2000; i++)
                MemoryFunctions::memcpy(from, to, 1024);

            console->WriteLine("Done");


            console->WriteLine("Test With SSE");
            CPU::SSE = true;

            void* from2 = MemoryManager::activeMemoryManager->malloc(1024);
            void* to2 = MemoryManager::activeMemoryManager->malloc(1024);

            for(int i = 0; i < 2000; i++)
                MemoryFunctions::memcpy(from2, to2, 1024);

            console->WriteLine("Done");

            MemoryManager::activeMemoryManager->free(from);
            MemoryManager::activeMemoryManager->free(to);

            MemoryManager::activeMemoryManager->free(from2);
            MemoryManager::activeMemoryManager->free(to2);
        }
    }

    this->console->CheckForScroll();
    MemoryManager::activeMemoryManager->free(input);
}