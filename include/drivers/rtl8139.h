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
        #define RX_BUF_SIZE 8192

        #define CAPR 0x38
        #define RX_READ_POINTER_MASK (~3)
        #define ROK                 (1<<0)
        #define RER                 (1<<1)
        #define TOK     (1<<2)
        #define TER     (1<<3)
        #define TX_TOK  (1<<15)

        enum RTL8139_registers {
            MAG0             = 0x00,       // Ethernet hardware address
            MAR0             = 0x08,       // Multicast filter
            TxStatus0        = 0x10,       // Transmit status (Four 32bit registers)
            TxAddr0          = 0x20,       // Tx descriptors (also four 32bit)
            RxBuf            = 0x30,
            RxEarlyCnt       = 0x34,
            RxEarlyStatus    = 0x36,
            ChipCmd          = 0x37,
            RxBufPtr         = 0x38,
            RxBufAddr        = 0x3A,
            IntrMask         = 0x3C,
            IntrStatus       = 0x3E,
            TxConfig         = 0x40,
            RxConfig         = 0x44,
            Timer            = 0x48,        // A general-purpose counter
            RxMissed         = 0x4C,        // 24 bits valid, write clears
            Cfg9346          = 0x50,
            Config0          = 0x51,
            Config1          = 0x52,
            FlashReg         = 0x54,
            GPPinData        = 0x58,
            GPPinDir         = 0x59,
            MII_SMI          = 0x5A,
            HltClk           = 0x5B,
            MultiIntr        = 0x5C,
            TxSummary        = 0x60,
            MII_BMCR         = 0x62,
            MII_BMSR         = 0x64,
            NWayAdvert       = 0x66,
            NWayLPAR         = 0x68,
            NWayExpansion    = 0x6A,

            // Undocumented registers, but required for proper operation
            FIFOTMS          = 0x70,        // FIFO Control and test
            CSCR             = 0x74,        // Chip Status and Configuration Register
            PARA78           = 0x78,
            PARA7c           = 0x7c,        // Magic transceiver parameter register
        };

        private:
            common::uint8_t mac_addr[6];
            char * rx_buffer;
            int tx_cur;
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