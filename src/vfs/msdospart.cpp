 
#include <vfs/msdospart.h>
#include <common/convert.h>
#include <vfs/fat.h>
#include <vfs/partitiontypes.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::filesystem;
 

void printf(char*);
void printfHex(uint8_t);
void printfHex32(uint32_t);

void MSDOSPartitionTable::ReadPartitions(myos::drivers::AdvancedTechnologyAttachment *hd)
{
    
    MasterBootRecord mbr;
    printf("Reading MBR\n");
    
    hd->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));
    
    if(mbr.magicnumber != 0xAA55)
    {
        printf("illegal MBR\n");
        return;
    }
    
    
    for(int i = 0; i < 4; i++)
    {
        if(mbr.primaryPartition[i].partition_id == 0x00)
            continue;
        
        printf("Partition "); printf(Convert::itoa(i));
        
        printf(" Size: "); printf(Convert::itoa(mbr.primaryPartition[i].length));
        
        if(mbr.primaryPartition[i].bootable == 0x80)
            printf(" Bootable: True Type: ");
        else
            printf(" Bootable: False Type: ");
        
        char* partname = GetPartitionTypeName(mbr.primaryPartition[i].partition_id); 
        printf(partname); printf("\n");
        
        if(partname == "Linux" || partname == "W95 FAT32")
        {
            FatFileSystem fat;
            fat.Initialize(hd, mbr.primaryPartition[i].start_lba);
        }
    }
    
}
