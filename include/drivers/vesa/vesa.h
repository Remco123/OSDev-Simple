#ifndef MYOS__DRIVERS__VESA_H
#define MYOS__DRIVERS__VESA_H

#include <common/types.h>
#include <drivers/vesa/realmode.h>
#include <common/memfunc.h>
#include <memorymanagement.h>
#include <common/convert.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>
#include <gui/canvas.h>
#include <gui/image.h>

namespace myos
{
    namespace drivers
    {
        struct VESA_INFO {
            char signature[4];   // must be "VESA" to indicate valid VBE support
            common::uint16_t version;         // VBE version; high byte is major version, low byte is minor version
            common::uint32_t oem;         // segment:offset pointer to OEM
            common::uint32_t capabilities;      // bitfield that describes card capabilities
            common::uint32_t video_modes;      // segment:offset pointer to list of supported video modes
            common::uint16_t video_memory;      // amount of video memory in 64KB blocks
            common::uint16_t software_rev;      // software revision
            common::uint32_t vendor;         // segment:offset to card vendor string
            common::uint32_t product_name;      // segment:offset to card model name
            common::uint32_t product_rev;      // segment:offset pointer to product revision
            char reserved[222];      // reserved for future expansion
            char oem_data[256];      // OEM BIOSes store their strings in this area
        }__attribute__ ((packed));

        struct MODE_INFO {
            common::uint16_t attributes;
            common::uint8_t windowA, windowB;
            common::uint16_t granularity;
            common::uint16_t windowSize;
            common::uint16_t segmentA, segmentB;
            common::uint32_t winFuncPtr;
            common::uint16_t pitch;

            common::uint16_t resolutionX, resolutionY;
            common::uint8_t wChar, yChar, planes, bpp, banks;
            common::uint8_t memoryModel, bankSize, imagePages;
            common::uint8_t reserved0;

            common::uint8_t readMask, redPosition;
            common::uint8_t greenMask, greenPosition;
            common::uint8_t blueMask, bluePosition;
            common::uint8_t reservedMask, reservedPosition;
            common::uint8_t directColorAttributes;

            // linear frame buffer
            common::uint32_t physbase;
            common::uint32_t offScreenMemOff;
            common::uint16_t offScreenMemSize;
            common::uint8_t  reserved1 [206];
            } __attribute__ ((packed));
        class Vesa
        {
            common::uint8_t* FrameBufferAddr;
        public:
            Vesa();
            
            void SelectMode(gui::Canvas* canv);
        };
    }
}

#endif