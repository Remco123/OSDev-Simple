#include <gui/shell.h>

#include <system.h>
#include <gui/arial.h>
#include <hardwarecommunication/acpi.h>


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

bool USE_SSE = false;

static inline void *memcpy(void *to, const void *from, unsigned int n)
{
   if(CPU::SSE)
   {
      int i;
      for(i=0; i<n/16; i++)
      {
         __asm__ __volatile__ ("movups (%0), %%xmm0\n" "movntdq %%xmm0, (%1)\n"::"r"(from), "r"(to) : "memory");

         from += 16;
         to += 16;
      }
   }
   if(n & 7)
   {
      n = n&7;

      int d0, d1, d2;
      __asm__ __volatile__(
      "rep ; movsl\n\t"
      "testb $2,%b4\n\t"
      "je 1f\n\t"
      "movsw\n"
      "1:\ttestb $1,%b4\n\t"
      "je 2f\n\t"
      "movsb\n"
      "2:"
      : "=&c" (d0), "=&D" (d1), "=&S" (d2)
      :"0" (n/4), "q" (n),"1" ((long) to),"2" ((long) from)
      : "memory");
   }
   return (to);
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
    else if(strcmp(input, "sse") == 1)
    {
        printf("Enabling SSE\n");

        printf("Not working yet\n");
        
        //USE_SSE = true;
    }
    else if(strcmp(input, "memtest") == 1)
    {
        printf("Memset Test\n");
        printf("Allocating 2, 20 Mb Buffers...\n");
        uint8_t* buf1 = (uint8_t*) MemoryManager::activeMemoryManager->malloc(1024 * 1024 * 20);
        uint8_t* buf2 = (uint8_t*) MemoryManager::activeMemoryManager->malloc(1024 * 1024 * 20);
        printf("Filling buf1 with 12...\n");
        MemoryFunctions::memset(buf1, 12, 1024 * 1024 * 20);
        printf("Copy Test...\n");
        if(USE_SSE)
            memcpy(buf2, buf1, 1024 * 1024 * 20);
        else
            MemoryFunctions::memcpy(buf2, buf1, 1024 * 1024 * 20);
        printf("Printing first 20 values of both buffers\n");
        for(int i = 0; i < 21; i++)
        {
            printf("Buf1: "); printf(Convert::itoa(buf1[i])); printf(" Buf2: "); printf(Convert::itoa(buf2[i])); printf("\n");
        }
        printf("Freeing buffers\n");
        MemoryManager::activeMemoryManager->free(buf1);
        MemoryManager::activeMemoryManager->free(buf2);
        printf("Done\n");
    }
    else if(strcmp(input, "videotest") == 1)
    {
        printf("Starting VESA Tests\n");
        System::canvas->Clear(Color::Create(44, 47, 51));

        Arial ar;
        ar.DrawTo(System::canvas, "Dit is een stukje tekst om de videomogelijkheden te testen van deze os\nZoals je ziet valt dit echt niet tegen alleen zou het wel fijn zijn als het wat sneller was.\n", 10, 30, 30, Color::Create(200, 200, 200));

        System::canvas->DrawFillCircle(Color::Create(255, 0, 0), System::canvas->Width/2 - 80, System::canvas->Height/2 - 40, 80);
        System::canvas->DrawFillCircle(Color::Create(0, 255, 0), System::canvas->Width/2, System::canvas->Height/2 - 40, 80);
        System::canvas->DrawFillCircle(Color::Create(0, 0, 255), System::canvas->Width/2 + 80, System::canvas->Height/2 - 40, 80);
    }
    else if(strcmp(input, "clear") == 1)
    {
        console->Clear();
    }
    else if(strcmp(input, "shutdown") == 1)
    {
        ACPI acpi;
        acpi.initAcpi();
        acpi.acpiPowerOff();
    }

    this->console->CheckForScroll();
    MemoryManager::activeMemoryManager->free(input);
}