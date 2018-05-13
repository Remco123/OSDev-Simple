#include <drivers/rtl8139.h>

using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;
using namespace myos::drivers;

void printf(char*);
void printfHex(uint8_t);
void printfHex32(uint32_t);

RTL8139::RTL8139(myos::hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor *dev,
                         myos::hardwarecommunication::InterruptManager* interrupts, PeripheralComponentInterconnectController* pci)
: InterruptHandler(interrupts, dev->interrupt + interrupts->HardwareInterruptOffset())
{
    
}
RTL8139::~RTL8139()
{

}

void RTL8139::Activate()
{
    
}
int RTL8139::Reset()
{
    
}
uint32_t RTL8139::HandleInterrupt(uint32_t esp)
{
    printf("Got Interrupt\n");
}

void RTL8139::Send(unsigned char *buffer, uint32_t len)
{
    
}
void RTL8139::Receive()
{

}