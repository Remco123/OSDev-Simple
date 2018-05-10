#include <vfs/fat.h>
#include <common/memfunc.h>
#include <memorymanagement.h>
#include <common/convert.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::filesystem;

void printf(char*);
void printfHex(uint8_t);

FatFileSystem::FatFileSystem()
{
    
}
void FatFileSystem::Initialize(AdvancedTechnologyAttachment *hd, uint32_t partitionOffset)
{
    fat_BS bpb;
    hd->Read28(partitionOffset, (uint8_t*)&bpb, sizeof(fat_BS));

    printf("sectors per cluster: ");
    printfHex(bpb.sectors_per_cluster);
    printf("\n");

    total_clusters = bpb.total_sectors_32 / bpb.sectors_per_cluster;
    first_data_sector = bpb.reserved_sector_count + bpb.table_count * bpb.table_size_32;
    first_fat_sector = bpb.reserved_sector_count;

    this->fatinfo = (fat_BS*)MemoryManager::activeMemoryManager->malloc(sizeof(fat_BS));
    MemoryFunctions::memcpy(this->fatinfo, &bpb, sizeof(fat_BS));
	
	DirectoryEntryFat32 dirent[16];
    hd->Read28(bpb.root_cluster, (uint8_t*)&dirent[0], 16*sizeof(DirectoryEntryFat32));
    printf("Files: "); printf(Convert::itoa(bpb.root_entry_count)); printf("\n");
    for(int i = 0; i < 16; i++)
    {
        if(dirent[i].name[0] == 0x00)
            break;
        
        if((dirent[i].attributes & 0x0F) == 0x0F)
            continue;
        
        printf((char*)dirent[i].name); printf("\n");
        
        
        if((dirent[i].attributes & 0x10) == 0x10) // directory
            continue;
	}
}