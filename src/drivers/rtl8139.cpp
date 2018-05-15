#include <drivers/rtl8139.h>

using namespace myos;
using namespace myos::common;
using namespace myos::hardwarecommunication;
using namespace myos::drivers;

void printf(char*);
void printfHex(uint8_t);
void printfHex16(uint16_t);
void printfHex32(uint32_t);

uint32_t current_packet_ptr;

// Four TXAD register, you must use a different one to send packet each time(for example, use the first one, second... fourth and back to the first)
uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};

static uint32_t BaseAddress=0x0;

RTL8139::RTL8139(myos::hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor *dev,
                         myos::hardwarecommunication::InterruptManager* interrupts, PeripheralComponentInterconnectController* pci)
: InterruptHandler(interrupts, dev->interrupt + interrupts->HardwareInterruptOffset())
{
    BaseAddress = pci->Read(dev->bus, dev->device, dev->function, 0x10);
    pci->Write(dev->bus, dev->device, dev->function, 0x10, ~0);
    uint32_t size = pci->Read(dev->bus, dev->device, dev->function, 0x10);
    pci->Write(dev->bus, dev->device, dev->function, 0x10,BaseAddress);

    // Enable PCI Bus Mastering
    uint32_t pci_command_reg = pci->Read(dev->bus, dev->device, dev->function, 0x04);
    if(!(pci_command_reg & (1 << 2))) {
        pci_command_reg |= (1 << 2);
        pci->Write(dev->bus, dev->device, dev->function, 0x04, pci_command_reg);
    }

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
        mac_addr[i] = inportb(BaseAddress + i);
        printfHex(mac_addr[i]); printf(" ");
    }
    printf("\n");
}
RTL8139::~RTL8139()
{
    
}

void RTL8139::Activate()
{
    outportb(BaseAddress + 0x52, 0x0);

    Reset();

    rx_buffer = (char*) MemoryManager::activeMemoryManager->malloc (8192 + 16 + 1500);
    MemoryFunctions::memset(rx_buffer, 0x0, 8192 + 16 + 1500);
    outportl(BaseAddress + 0x30, (uint32_t)rx_buffer);

    // Sets the TOK and ROK bits high
    outportw(BaseAddress + 0x3C, 0x0005);

    // (1 << 7) is the WRAP bit, 0xf is AB+AM+APM+AAP
    outportl(BaseAddress + 0x44, 0xf | (1 << 7));

    // Sets the RE and TE bits high
    outportb(BaseAddress + 0x37, 0x0C);
}
int RTL8139::Reset()
{
    // Soft reset
    outportb(BaseAddress + 0x37, 0x10);
    while((inportb(BaseAddress + 0x37) & 0x10) != 0) {
        // Do nothibg here...
    }
}
uint32_t RTL8139::HandleInterrupt(uint32_t esp)
{
    printf("Got Interrupt\n");
    
    uint16_t status = inportw(BaseAddress + 0x3e);

    if(status & TOK) {
        printf("Packet sent\n");
    }
    if (status & ROK) {
        printf("Received packet\n");
        Receive();
    }

    outportw(BaseAddress + 0x3E, 0x5);

    printf("Done\n");
    return esp;
}

void RTL8139::Send(unsigned char *buffer, uint32_t len)
{
    void * transfer_data = MemoryManager::activeMemoryManager->malloc(len);
    MemoryFunctions::memcpy(transfer_data, buffer, len);

    // Second, fill in physical address of data, and length
    outportl(BaseAddress + TSAD_array[tx_cur], (uint32_t)transfer_data);
    outportl(BaseAddress + TSD_array[tx_cur++], len);
    if(tx_cur > 3)
        tx_cur = 0;

    printf("Packet Should be Send\n");
}
void RTL8139::Receive()
{
    uint16_t * t = (uint16_t*)(rx_buffer + current_packet_ptr);
    // Skip packet header, get packet length
    uint16_t packet_length = *(t + 1);

    // Skip, packet header and packet length, now t points to the packet data
    t = t + 2;
    printf("Printing packet at addr 0x"); printfHex32((uint32_t)t); printf("\n");
    for(uint16_t i = 0; i < packet_length; i++)
    {
        printfHex16(t[i]); printf(" ");
    }

    // Now, ethernet layer starts to handle the packet(be sure to make a copy of the packet, insteading of using the buffer)
    // and probabbly this should be done in a separate thread...
    void * packet = MemoryManager::activeMemoryManager->malloc(packet_length);
    MemoryFunctions::memcpy(packet, t, packet_length);
    //ethernet_handle_packet(packet, packet_length);

    current_packet_ptr = (current_packet_ptr + packet_length + 4 + 3) & RX_READ_POINTER_MASK;

    if(current_packet_ptr > RX_BUF_SIZE)
        current_packet_ptr -= RX_BUF_SIZE;

    outportw(BaseAddress + CAPR, current_packet_ptr - 0x10);
}