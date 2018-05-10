#ifndef  MYOS__VFS__PARTITIONTYPES_H
#define  MYOS__VFS__PARTITIONTYPES_H

#include <common/types.h>

namespace myos
{
    namespace filesystem
    {
        static char* GetPartitionTypeName(common::uint8_t id)
        {
            switch(id)
            {
                case 0:
                    return "Empty";
                case 1:
                    return "FAT12";
                case 4:
                    return "FAT16 <32M";
                case 5:
                    return "Extended";
                case 6:
                    return "FAT16";
                case 0xb:
                    return "W95 FAT32";
                case 0xc:
                    return "W95 FAT32 (LBA)";
                case 0xe:
                    return "W95 FAT16 (LBA)";
                case 0x82:
                    return "Linux swap";
                case 0x83:
                    return "Linux";
                default:
                    return "Unkown";
            }
        }
    }
}

#endif