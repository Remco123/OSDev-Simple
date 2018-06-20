#include <drivers/pit.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;

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