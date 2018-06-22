#ifndef MYOS__ACPI_H
#define MYOS__ACPI_H

#include <common/types.h>
#include <common/memfunc.h>
#include <hardwarecommunication/port.h>
#include <system.h>

namespace myos
{
    class ACPI
    {
        private:
            common::uint32_t *SMI_CMD;
            common::uint8_t ACPI_ENABLE;
            common::uint8_t ACPI_DISABLE;
            common::uint32_t *PM1a_CNT;
            common::uint32_t *PM1b_CNT;
            common::uint16_t SLP_TYPa;
            common::uint16_t SLP_TYPb;
            common::uint16_t SLP_EN;
            common::uint16_t SCI_EN;
            common::uint8_t PM1_CNT_LEN;

            struct RSDPtr
            {
                common::uint8_t Signature[8];
                common::uint8_t CheckSum;
                common::uint8_t OemID[6];
                common::uint8_t Revision;
                common::uint32_t *RsdtAddress;
            };
            struct FACP
            {
                common::uint8_t Signature[4];
                common::uint32_t Length;
                common::uint8_t unneded1[40 - 8];
                common::uint32_t *DSDT;
                common::uint8_t unneded2[48 - 44];
                common::uint32_t *SMI_CMD;
                common::uint8_t ACPI_ENABLE;
                common::uint8_t ACPI_DISABLE;
                common::uint8_t unneded3[64 - 54];
                common::uint32_t *PM1a_CNT_BLK;
                common::uint32_t *PM1b_CNT_BLK;
                common::uint8_t unneded4[89 - 72];
                common::uint8_t PM1_CNT_LEN;
            };
        public:

            // check if the given address has a valid header
            unsigned int *acpiCheckRSDPtr(unsigned int *ptr);
            // finds the acpi header and returns the address of the rsdt
            unsigned int *acpiGetRSDPtr(void);
            // checks for a given header and validates checksum
            int acpiCheckHeader(unsigned int *ptr, char *sig);
            int acpiEnable();
            int initAcpi(void);
            void acpiPowerOff(void);
    };
}

#endif