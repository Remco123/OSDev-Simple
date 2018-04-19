#ifndef __MYOS__DRIVERS__SERIAL_H
#define __MYOS__DRIVERS__SERIAL_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>

namespace myos
{
    namespace drivers
    {
        enum SerialCom : common::uint16_t
        {
            COM1 = 0x3F8,
            COM2 = 0x2F8,
            COM3 = 0x3E8,
            COM4 = 0x2E8
        };

        class SerialPort
        {
        public:
            SerialPort(SerialCom port);
            SerialCom port;

            bool DataAvailibe();
            void Send(common::uint8_t* data);
            common::uint8_t* Receive();
        };  

        class SerialPortManager : public hardwarecommunication::InterruptHandler
        {
        protected:
            void EnableSerial(SerialCom port);
        public:
            SerialPortManager(myos::hardwarecommunication::InterruptManager* manager);

            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
            SerialPort GetInstance(SerialCom port);
        };
    }
}

#endif