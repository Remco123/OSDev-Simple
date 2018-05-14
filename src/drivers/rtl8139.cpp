#include <drivers/rtl8139.h>

using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;
using namespace myos::drivers;

void printf(char*);
void printfHex(uint8_t);
void printfHex32(uint32_t);

void sleep(uint32_t time)
{
    int i;
    for(i=0; i<10000000*time; i++)
        ;
}

unsigned char rxBuffer[1500];
unsigned char TxBuffer[1300];

static uint32_t BaseAddress=0x0;

RTL8139::RTL8139(myos::hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor *dev,
                         myos::hardwarecommunication::InterruptManager* interrupts, PeripheralComponentInterconnectController* pci)
: InterruptHandler(interrupts, dev->interrupt + interrupts->HardwareInterruptOffset())
{
    BaseAddress = pci->Read(dev->bus, dev->device, dev->function, 0x10);
    pci->Write(dev->bus, dev->device, dev->function, 0x10, ~0);
    uint32_t size = pci->Read(dev->bus, dev->device, dev->function, 0x10);
    pci->Write(dev->bus, dev->device, dev->function, 0x10,BaseAddress);

    if( (BaseAddress & 0x1) == 1)
    {
        printf("Detected I/O type \n");
        BaseAddress = BaseAddress & 0xfffffffc ;
    }
    else
    {
        printf("Detected unknown mapped type will do dump\n");
        return;
    }

    //read mac
    printf("Card Mac Address = ");
    for(unsigned int i=0; i<6; i++)
    {
        macAddress[i] = inportb(BaseAddress + i);
        printfHex(macAddress[i]); printf(" ");
    }
    printf("\n");
}
RTL8139::~RTL8139()
{
    
}

void RTL8139::Activate()
{
    //power on
    outportb( (uint16_t) (BaseAddress + 0x52), 0x00);
    //reset
    Reset();

    outportw((BaseAddress + INT_STATUS_REG), 0xe1ff);
    outportw((BaseAddress + PACKET_READ_ADDRESS), 0);
    // Setup receive buffer location
    outportl((BaseAddress + RECEIVE_BUFFER_ADDRESS), (uint32_t)rxBuffer );
    outportw((BaseAddress + INT_MASK_REG), 0xe1ff);  // TOK + ROK
    outportl((BaseAddress + RECEIVE_CONFIG_REG), 0xa);// | (1<<7) );
    outportb( (BaseAddress + COMMANDREGISTER), 0x0C );
}
int RTL8139::Reset()
{
    outportb( (uint16_t) (BaseAddress + COMMANDREGISTER), 0x10);
    while ( (inportb( (uint16_t) (BaseAddress + COMMANDREGISTER)) & 0x10) != 0 ) {
        printf("Waiting for resetting rtl8139 card\n");
    }
}
uint32_t RTL8139::HandleInterrupt(uint32_t esp)
{
    printf("Got Interrupt\n");
    outportw(BaseAddress + INT_MASK_REG, 0);
    uint32_t interruptStatus = inportw(BaseAddress + INT_STATUS_REG);
    printf("Status of Interrupt Status = 0x"); printfHex32(interruptStatus); printf("\n");
    outportw(BaseAddress + INT_STATUS_REG, interruptStatus); //reset
    if( interruptStatus & 0x01 ) {
        printf("Packet received\n");
        //ReceivePacketFromWire(rxBuffer);
    }
    if( interruptStatus & 0x04 ) {
        printf("Transmitted a packet sussessfully\n");
    }
    outportw((BaseAddress + PACKET_READ_ADDRESS), 0);
    outportl((BaseAddress + RECEIVE_BUFFER_ADDRESS), (uint32_t)rxBuffer );
    outportw((BaseAddress + INT_MASK_REG), 0xe1ff); // TOK + ROK
    outportl((BaseAddress + RECEIVE_CONFIG_REG), 0xa);//
    printf("Done\n");
    return esp;
}

void RTL8139::Send(unsigned char *buffer, uint32_t len)
{
    static uint32_t currentStartDesc = TRSMIT_START_DESC;
    static uint32_t currentStatusDesc = TRSMIT_STATUS_DESC;
    static int offset = 0;
    offset = offset + 4;
    uint32_t value = (uint32_t)(&TxBuffer[0]) - (uint32_t)(&TxBuffer[0]) % 32 + 32;
    sleep(2);
    uint32_t status = 0x003f0000 | len;
    outportl((BaseAddress + currentStartDesc), (unsigned int) buffer);
    outportl((BaseAddress + currentStatusDesc),status);
    currentStartDesc = TRSMIT_START_DESC+(offset%20);
    currentStatusDesc = TRSMIT_STATUS_DESC+(offset%20);

    printf("Packet Should be Send\n");
}
void RTL8139::Receive()
{

}