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