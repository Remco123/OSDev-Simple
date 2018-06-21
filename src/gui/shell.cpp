#include <gui/shell.h>

#include <system.h>


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
    else if(strcmp(input, "beep") == 1)
        System::pit->Beep(700, 250);
    else if(strcmp(input, "time") == 1)
    {
        printf("Time: "); printf(Convert::itoa(System::rtc->GetHour())); printf(":"); printf(Convert::itoa(System::rtc->GetMinute())); printf(":"); printf(Convert::itoa(System::rtc->GetSecond())); printf("\n");
    }
    else if(strcmp(input, "date") == 1)
    {
        printf("Date: "); printf(Convert::itoa(System::rtc->GetYear())); printf(":"); printf(Convert::itoa(System::rtc->GetMonth())); printf(":"); printf(Convert::itoa(System::rtc->GetDay())); printf("\n");
    }

    this->console->CheckForScroll();
    MemoryManager::activeMemoryManager->free(input);
}