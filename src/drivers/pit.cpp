#include <drivers/pit.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

void printf(char*);

PIT::PIT(hardwarecommunication::InterruptManager* manager)
: DataPort(0x40),
  CommandPort(0x43),
  InterruptHandler(manager, manager->HardwareInterruptOffset() + 0)
{
    timer_ticks = 0;

    uint64_t divisor = 1193180 / 1000;

    CommandPort.Write(0x36);
    DataPort.Write((uint8_t)divisor);
    DataPort.Write((uint8_t)(divisor >> 8));
}

uint32_t PIT::HandleInterrupt(uint32_t esp)
{
    timer_ticks++;

    return esp;
}
void PIT::Sleep(uint32_t ms)
{
    //MS is the same as ticks because freq = 1000
    unsigned long eticks;

    eticks = timer_ticks + ms;
    while(timer_ticks < eticks);
}


void PIT::PlaySound(common::uint32_t nFrequence)
{
    uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outportb(0x43, 0xb6);
 	outportb(0x42, (uint8_t) (Div) );
 	outportb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inportb(0x61);
  	if (tmp != (tmp | 3)) {
 		outportb(0x61, tmp | 3);
 	}
}
void PIT::NoSound()
{
    uint8_t tmp = inportb(0x61) & 0xFC;
 
 	outportb(0x61, tmp);
}
void PIT::Beep()
{
    Beep(1000); //1000 is default beep frequency
}

void PIT::Beep(common::uint32_t freq)
{
    Beep(freq, 10); //10 is default beep duration
}
void PIT::Beep(common::uint32_t freq, common::uint32_t duration)
{
    if(duration == 0)
        return;

    PlaySound(freq);
 	Sleep(duration);
 	NoSound();
}