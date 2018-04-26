#include <hardwarecommunication/cpu.h>
#include <common/convert.h>

using namespace myos::common;
using namespace myos::hardwarecommunication;


#define HZ		100
#define LATCH 		((1193180 + HZ / 2) / HZ)
#define CALIBRATE_LATCH (5 * LATCH)
#define CALIBRATE_TIME	(5 * 1000020 / HZ)



void printf(char* str,...);

/* Input a byte from a port */
inline unsigned char inportb(unsigned int port)
{
   unsigned char ret;
   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}

/* Output a byte to a port */
/* July 6, 2001 added space between :: to make code compatible with gpp */

inline void outportb(unsigned int port,unsigned char value)
{
   asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}


static inline void cpuid(uint32_t reg, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    __asm__ volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}

//Read the timestamp counter
static inline void rdtsc(unsigned long *low, unsigned long *high)
{
	asm volatile( "rdtsc" : "=a" ( *low ), "=d" ( *high ) );
}


static unsigned long calibrate_tsc( void )
{
	unsigned long startlow, starthigh, endlow, endhigh, count;

	outportb( 0x61, ( inportb( 0x61 ) & ~0x02 ) | 0x01 );
	outportb( 0x43, 0xb0 );
	outportb( 0x42, CALIBRATE_LATCH & 0xff );
	outportb( 0x42, CALIBRATE_LATCH >> 8 );
	rdtsc( &startlow, &starthigh );
	count = 0;
	do {
		count++;
	} while ( ( inportb( 0x61 ) & 0x20 ) == 0 );
	rdtsc( &endlow, &endhigh );
 	if ( count <= 1 ) {
		return 0;
	}
	asm ( "subl %2, %0\n"
		"sbbl %3,%1" : "=a" ( endlow ), "=d" ( endhigh ) :
		"g" ( startlow ), "g" ( starthigh ), "0" ( endlow ), "1" ( endhigh ) );
	if ( endhigh != 0 ) {
		return 0;
	}
	if ( endlow <= CALIBRATE_TIME ) {
		return 0;
	}

	asm ( "divl %2" : "=a" ( endlow ), "=d" ( endhigh ) :
		"r" ( endlow ),"0" ( 0 ), "1" ( CALIBRATE_TIME ) );

	return endlow;
}


int CPU::GetSpeed()
{
	unsigned long tsc_quotient = calibrate_tsc();
	unsigned long cpu_hz = 0;

	if(tsc_quotient != 0)
	{
		unsigned long eax = 0, edx = 1000000;
		asm ( "divl %2" : "=a" ( cpu_hz ), "=d" ( edx ) :
			"r" ( tsc_quotient ), "0" ( eax ), "1" ( edx ) );
	}
	return cpu_hz / 1000000;
}

void CPU::PrintInfo()
{
    // Register storage
    uint32_t eax, ebx, ecx, edx;

    // Function 0x00 - Vendor-ID and Largest Standard Function

    uint32_t largestStandardFunc;
    char vendor[13];
    cpuid(0, &largestStandardFunc, (uint32_t*)(vendor + 0), (uint32_t *)(vendor + 8), (uint32_t *)(vendor + 4));
    vendor[12] = '\0';

    printf("CPU Vendor: "); printf(vendor); printf("\n");

    // Function 0x01 - Feature Information

    if (largestStandardFunc >= 0x01)
    {
        cpuid(0x01, &eax, &ebx, &ecx, &edx);

        printf("Features:");

        if (edx & EDX_PSE)      printf(" PSE");
        if (edx & EDX_PAE)      printf(" PAE");
        if (edx & EDX_APIC)     printf(" APIC");
        if (edx & EDX_MTRR)     printf(" MTRR");

        printf("\n");

        printf("Instructions:");

        if (edx & EDX_TSC)      printf(" TSC");
        if (edx & EDX_MSR)      printf(" MSR");
        if (edx & EDX_SSE)      printf(" SSE");
        if (edx & EDX_SSE2)     printf(" SSE2");
        if (ecx & ECX_SSE3)     printf(" SSE3");
        if (ecx & ECX_SSSE3)    printf(" SSSE3");
        if (ecx & ECX_SSE41)    printf(" SSE41");
        if (ecx & ECX_SSE42)    printf(" SSE42");
        if (ecx & ECX_AVX)      printf(" AVX");
        if (ecx & ECX_F16C)     printf(" F16C");
        if (ecx & ECX_RDRAND)   printf(" RDRAND");

        printf("\n");
    }

    // Extended Function 0x00 - Largest Extended Function

    uint32_t largestExtendedFunc;
    cpuid(0x80000000, &largestExtendedFunc, &ebx, &ecx, &edx);

    // Extended Function 0x01 - Extended Feature Bits

    if (largestExtendedFunc >= 0x80000001)
    {
        cpuid(0x80000001, &eax, &ebx, &ecx, &edx);

        if (edx & EDX_64_BIT)
        {
            printf("64-bit Architecture\n");
        }
    }

    // Extended Function 0x02-0x04 - Processor Name / Brand String

    if (largestExtendedFunc >= 0x80000004)
    {
        char name[48];
        cpuid(0x80000002, (uint32_t *)(name +  0), (uint32_t *)(name +  4), (uint32_t *)(name +  8), (uint32_t *)(name + 12));
        cpuid(0x80000003, (uint32_t *)(name + 16), (uint32_t *)(name + 20), (uint32_t *)(name + 24), (uint32_t *)(name + 28));
        cpuid(0x80000004, (uint32_t *)(name + 32), (uint32_t *)(name + 36), (uint32_t *)(name + 40), (uint32_t *)(name + 44));

        // Processor name is right justified with leading spaces
        char *p = name;
        while (*p == ' ')
        {
            ++p;
        }

        printf("CPU Name: "); printf(p); printf("\n");
    }

    printf("Speed: ");
    printf(Convert::itoa(GetSpeed()));
    printf("\n");
}