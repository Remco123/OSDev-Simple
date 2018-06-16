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
void printfHex16(uint16_t);

//Converts the file name stored in a FAT directory entry into a more human-sensible format
void convertFromFATFormat(char* input, char* output)
{
	unsigned short counter = 0;

	//iterate through the 8 letter file name, adding a dot when the end is reached
	for ( counter = 0; counter < 8; counter++)
	{
		if (input[counter] == 0x20)
		{
			output[counter] = '.';
			break;
		}

		output[counter] = input[counter];
	}

	//if the entire 8 letters of the file name were used, tack a dot onto the end
	if (counter == 8)
	{
		output[counter] = '.';
	}

	unsigned short counter2 = 8;

	//iterate through the three-letter extension, adding it on. (Note: if the input is a directory (which has no extension) it erases the dot put in previously)
	for (counter2 = 8; counter2 < 11; counter2++)
	{
		++counter;
		if (input[counter2] == 0x20 || input[counter2] == 0x20)
		{
			if (counter2 == 8) //there is no extension, the dot added earlier must be removed
				counter -= 2; //it's minus two because the for loop above iterates the loop as well
			break;
		}
		output[counter] = input[counter2];		
	}

	++counter;
	while (counter < 12)
	{
		output[counter] = ' ';
		++counter;
	}

	output[12] = '\0'; //ensures proper termination regardless of program operation previously
	return;
}

void trimName(char *str, size_t size) {
    if (!size)
        return;

    for (size_t i = size-1; ; i--) {
        if (str[i] == ' ')
            str[i] = '\0';
        else if (str[i])
            break;

        if (i == 0)
            break;
    }
}

void printtime(uint16_t date, uint16_t time)
{
    //Hour
    printf(Convert::itoa((time & 0xFE00) >> 11)); printf(":");
    //Minute
    printf(Convert::itoa((time & 0x1E0) >> 5)); printf(":");
    //Sec
    printf(Convert::itoa((time & 0x1F) << 1)); printf(" ; ");

    //Day
    printf(Convert::itoa(date & 0x1F)); printf(":");
    //Month
    printf(Convert::itoa(((date & 0x1E0) >> 5))); printf(":");
    //Year
    printf(Convert::itoa(((date & 0xFE00) >> 9) + 1980));
}

FatFileSystem::FatFileSystem()
{
    
}
void FatFileSystem::Initialize(AdvancedTechnologyAttachment *hd, uint32_t partitionOffset)
{
    this->hd = hd;

    BiosParameterBlock32 bpb;
    hd->Read28(partitionOffset, (uint8_t*)&bpb, sizeof(BiosParameterBlock32));

    
    printf("sectors per cluster: ");
    printf(Convert::itoa(bpb.sectorsPerCluster));
    printf("\n");

    printf("bytes per sector: ");
    printf(Convert::itoa(bpb.bytesPerSector));
    printf("\n");

    printf("cluster size: ");
    printf(Convert::itoa(bpb.sectorsPerCluster * bpb.bytesPerSector));
    printf("\n");

    printf("Number of fats: "); printf(Convert::itoa(bpb.fatCopies)); printf("\n");

    printf("Label: "); printf((char*)bpb.volumeLabel); printf("\n");
    printf("Volume ID: "); printf(Convert::itoa(bpb.volumeId)); printf("\n");
    
    fatStart = partitionOffset + bpb.reservedSectors;
    fatSize = bpb.tableSize;
    printf("Size of 1 fat: "); printf(Convert::itoa(bpb.tableSize)); printf("\n");

    dataStart = fatStart + fatSize*bpb.fatCopies;

    rootStart = dataStart + bpb.sectorsPerCluster*(bpb.rootCluster - 2);
    sectorsPerCluster = bpb.sectorsPerCluster;
}

void FatFileSystem::ListRootDir()
{
    printf("#-------------------#\n");
    DirectoryEntryFat32 dirent[16];
    hd->Read28(rootStart, (uint8_t*)&dirent[0], 16*sizeof(DirectoryEntryFat32));
    
    for(int i = 0; i < 16; i++)
    {
        if(dirent[i].name[0] == 0x00)
            break;
        
        if((dirent[i].attributes & 0x0F) == 0x0F)
            continue;

        char* foo = "          ";
        convertFromFATFormat((char*)dirent[i].name, foo);
        
        printf(foo); printf("     Size: "); printf(Convert::itoa(dirent[i].size));
        printf(" ");
        //printtime(dirent[i].cDate, dirent[i].cTime);
        printf("\n");
        
        if((dirent[i].attributes & 0x10) == 0x10) // directory
            continue;
    }
    printf("#-------------------#\n");
}


int strcmp2(const char *s1, const char *s2)
{
    while ((*s1 == *s2) && *s1) { ++s1; ++s2; }
    return (((int) (unsigned char) *s1) - ((int) (unsigned char) *s2) == 0) ? 1 : 0;
}

void FatFileSystem::GetFile(char* name, uint8_t* buffer) //Only works in root dir right now
{
    DirectoryEntryFat32 dirent[16];
    hd->Read28(rootStart, (uint8_t*)&dirent[0], 16*sizeof(DirectoryEntryFat32));
    
    for(int i = 0; i < 16; i++)
    {
        if(dirent[i].name[0] == 0x00)
            break;
        
        if((dirent[i].attributes & 0x0F) == 0x0F)
            continue;

        char* realname;
        convertFromFATFormat((char*)dirent[i].name, realname);

        trimName(realname, 11);

        if(strcmp2(realname, name) == 1)
        {
            uint32_t firstFileCluster = ((uint32_t)dirent[i].firstClusterHi) << 16
                                | ((uint32_t)dirent[i].firstClusterLow);               
                                
            int32_t SIZE = dirent[i].size;
            int32_t nextFileCluster = firstFileCluster;
            uint8_t fatbuffer[513];
            uint8_t tempbuffer[513];
            int32_t bytesRead = 0;
                                  
            while(SIZE > 0)
            {
                uint32_t fileSector = dataStart + sectorsPerCluster * (nextFileCluster-2);
                int sectorOffset = 0;

                printf("Reading Sector: "); printf(Convert::itoa(fileSector)); printf(" Cluster: "); printf(Convert::itoa(nextFileCluster - 2)); printf("\n");
            
                for(; SIZE > 0; SIZE -= 512)
                {
                    hd->Read28(fileSector+sectorOffset, tempbuffer, 512);
                    
                    tempbuffer[SIZE > 512 ? 512 : SIZE] = '\0';

                    for(int i = 0; i < 512; i++)
                    {
                        buffer[bytesRead + i] = tempbuffer[i];
                    }

                    printf((char*)tempbuffer);

                    bytesRead += (SIZE > 512 ? 512 : SIZE);
                    
                    if(++sectorOffset > sectorsPerCluster)
                        break;
                }                
                uint32_t fatSectorForCurrentCluster = nextFileCluster / (512/sizeof(uint32_t));
                hd->Read28(fatStart+fatSectorForCurrentCluster, fatbuffer, 512);
                uint32_t fatOffsetInSectorForCurrentCluster = nextFileCluster % (512/sizeof(uint32_t));
                nextFileCluster = ((uint32_t*)&fatbuffer)[fatOffsetInSectorForCurrentCluster] & 0x0FFFFFFF;
            }
            break;
        }
    }
}