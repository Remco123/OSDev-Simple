#ifndef MYOS__FAT__H
#define MYOS__FAT__H

#include <common/types.h>
#include <vfs/ata.h>

namespace myos
{
    namespace filesystem
    {
        struct BiosParameterBlock32
        {
            common::uint8_t jump[3];
            common::uint8_t softName[8];
            common::uint16_t bytesPerSector;
            common::uint8_t sectorsPerCluster;
            common::uint16_t reservedSectors;
            common::uint8_t fatCopies;
            common::uint16_t rootDirEntries;
            common::uint16_t totalSectors;
            common::uint8_t mediaType;
            common::uint16_t fatSectorCount;
            common::uint16_t sectorsPerTrack;
            common::uint16_t headCount;
            common::uint32_t hiddenSectors;
            common::uint32_t totalSectorCount;
            
            common::uint32_t tableSize;
            common::uint16_t extFlags;
            common::uint16_t fatVersion;
            common::uint32_t rootCluster;
            common::uint16_t fatInfo;
            common::uint16_t backupSector;
            common::uint8_t reserved0[12];
            common::uint8_t driveNumber;
            common::uint8_t reserved;
            common::uint8_t bootSignature;
            common::uint32_t volumeId;
            common::uint8_t volumeLabel[11];
            common::uint8_t fatTypeLabel[8];
        } __attribute__((packed));

        struct DirectoryEntryFat32
        {
            common::uint8_t name[8];
            common::uint8_t ext[3];
            common::uint8_t attributes;
            common::uint8_t reserved;
            common::uint8_t cTimeTenth;
            common::uint16_t cTime;
            common::uint16_t cDate;
            common::uint16_t aTime;
            common::uint16_t firstClusterHi;
            common::uint16_t wTime;
            common::uint16_t wDate;
            common::uint16_t firstClusterLow;
            common::uint32_t size;
        } __attribute__((packed));
        
        class FatFileSystem
        {
        private:
            common::uint32_t fatStart; 
            common::uint32_t fatSize;
            common::uint32_t dataStart;
            common::uint32_t rootStart;
            common::uint32_t sectorsPerCluster;

            drivers::AdvancedTechnologyAttachment* hd;
        public:
            FatFileSystem();
            void Initialize(myos::drivers::AdvancedTechnologyAttachment *hd, common::uint32_t partitionOffset);
            void ListRootDir();
            void GetFile(char* name, common::uint8_t* buffer); //Only works in root dir right now
        };
    }
}

#endif