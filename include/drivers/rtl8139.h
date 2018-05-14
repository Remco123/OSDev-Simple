#ifndef __MYOS__DRIVERS__RTL8139_h
#define __MYOS__DRIVERS__RTL8139_h

#include <common/types.h>
#include <common/memfunc.h>
#include <common/convert.h>

#include <hardwarecommunication/port.h>
#include <hardwarecommunication/pci.h>
#include <hardwarecommunication/pci.h>

namespace myos
{
    namespace drivers
    {
        class RTL8139 : public Driver, public hardwarecommunication::InterruptHandler
        {
        #define CONFIG1_REGISTER 0x52
        #define COMMANDREGISTER  0x37
        #define INT_MASK_REG	0x3c
        #define INT_STATUS_REG 0x3e
        #define PACKET_READ_ADDRESS 0x38
        #define RECEIVE_CONFIG_REG 0x44
        #define TRSMIT_START_DESC 0x20
        #define TRSMIT_STATUS_DESC 0x10
        #define RECEIVE_BUFFER_ADDRESS 0x30
        private:
            common::uint8_t macAddress[5];
        public:
            RTL8139(myos::hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor *dev,
                         myos::hardwarecommunication::InterruptManager* interrupts, myos::hardwarecommunication::PeripheralComponentInterconnectController* pci);
            ~RTL8139();
            
            void Activate();
            int Reset();
            common::uint32_t HandleInterrupt(common::uint32_t esp);
            
            void Send(unsigned char *buffer, common::uint32_t len);
            void Receive();
        };
    }
}

#endif