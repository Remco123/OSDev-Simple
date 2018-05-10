#ifndef MYOS__FAT__H
#define MYOS__FAT__H

#include <common/types.h>
#include <vfs/ata.h>

namespace myos
{
    namespace filesystem
    {
        //FAT constant values
        #define END_CLUSTER_32 0x0FFFFFF8 //Use OSDev.org's suggestion of 0x0FFFFFF8 even though MSYS docs > OSdev.org.
        #define BAD_CLUSTER_32 0x0FFFFFF7
        #define FREE_CLUSTER_32 0x00000000
        #define END_CLUSTER_16 0xFFF8
        #define BAD_CLUSTER_16 0xFFF7
        #define FREE_CLUSTER_16 0x0000
        #define END_CLUSTER_12 0xFF8
        #define BAD_CLUSTER_12 0xFF7
        #define FREE_CLUSTER_12 0x000

        #define CLEAN_EXIT_BMASK_16 0x8000
        #define HARD_ERR_BMASK_16 0x4000
        #define CLEAN_EXIT_BMASK_32 0x08000000
        #define HARD_ERR_BMASK_32 0x04000000

        #define FILE_READ_ONLY 0x01
        #define FILE_HIDDEN 0x02
        #define FILE_SYSTEM 0x04
        #define FILE_VOLUME_ID 0x08
        #define FILE_DIRECTORY 0x10
        #define FILE_ARCHIVE 0x20
        #define FILE_LONG_NAME (FILE_READ_ONLY|FILE_HIDDEN|FILE_SYSTEM|FILE_VOLUME_ID)
        #define FILE_LONG_NAME_MASK (FILE_READ_ONLY|FILE_HIDDEN|FILE_SYSTEM|FILE_VOLUME_ID|FILE_DIRECTORY|FILE_ARCHIVE)
        #define FILE_LAST_LONG_ENTRY 0x40
        #define ENTRY_FREE 0xE5
        #define ENTRY_END 0x00
        #define ENTRY_JAPAN 0x05
        #define LAST_LONG_ENTRY 0x40

        #define LOWERCASE_ISSUE	0x01 //E.g.: "test    txt"
        #define BAD_CHARACTER	0x02 //E.g.: "tes&t   txt"
        #define BAD_TERMINATION 0x04 //missing null character at the end
        #define NOT_CONVERTED_YET 0x08 //still contains a dot: E.g."test.txt"
        #define TOO_MANY_DOTS 0x10 //E.g.: "test..txt"; may or may not have already been converted

        #define GET_CLUSTER_FROM_ENTRY(x) (x.low_bits | (x.high_bits << (fat_type / 2)))
        #define GET_ENTRY_LOW_BITS(x) (x & ((fat_type /2) -1))
        #define GET_ENTRY_HIGH_BITS(x) (x >> (fat_type / 2))
        #define CONCAT_ENTRY_HL_BITS(high, low) ((high << (fat_type / 2)) | low)

        typedef struct fat_BS
        {
            unsigned char 		bootjmp[3];
            unsigned char 		oem_name[8];
            unsigned short 	    bytes_per_sector;
            unsigned char		sectors_per_cluster;
            unsigned short		reserved_sector_count;
            unsigned char		table_count;
            unsigned short		root_entry_count;
            unsigned short		total_sectors_16;
            unsigned char		media_type;
            unsigned short		table_size_16;
            unsigned short		sectors_per_track;
            unsigned short		head_side_count;
            unsigned int 		hidden_sector_count;
            unsigned int 		total_sectors_32;
            
            unsigned int		table_size_32;
            unsigned short		extended_flags;
            unsigned short		fat_version;
            unsigned int		root_cluster;
            unsigned short		fat_info;
            unsigned short		backup_BS_sector;
            unsigned char 		reserved_0[12];
            unsigned char		drive_number;
            unsigned char 		reserved_1;
            unsigned char		boot_signature;
            unsigned int 		volume_id;
            unsigned char		volume_label[11];
            unsigned char	    fat_type_label[8];

        }__attribute__((packed));

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
            unsigned int first_fat_sector;
            unsigned int first_data_sector;
            unsigned int total_clusters;

            fat_BS* fatinfo;
        public:
            FatFileSystem();
            void Initialize(myos::drivers::AdvancedTechnologyAttachment *hd, common::uint32_t partitionOffset);
        };
    }
}

#endif