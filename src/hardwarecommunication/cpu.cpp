#include <hardwarecommunication/cpu.h>
#include <common/convert.h>

using namespace myos::common;
using namespace myos::hardwarecommunication;


#define HZ		100
#define LATCH 		((1193180 + HZ / 2) / HZ)
#define CALIBRATE_LATCH (5 * LATCH)
#define CALIBRATE_TIME	(5 * 1000020 / HZ)

bool CPU::SSE3      = false;
bool CPU::PCLMULQDQ = false;
bool CPU::DTES64    = false;
bool CPU::MONITOR   = false;
bool CPU::DS_CPL    = false;
bool CPU::VMX       = false;
bool CPU::SMX       = false;
bool CPU::EST       = false;
bool CPU::TM2       = false;
bool CPU::SSSE3     = false;
bool CPU::CNXT_ID   = false;
bool CPU::FMA       = false;
bool CPU::CX16      = false;
bool CPU::XTPR      = false;
bool CPU::PDCM      = false;
bool CPU::PCID      = false;
bool CPU::DCA       = false;
bool CPU::SSE41     = false;
bool CPU::SSE42     = false;
bool CPU::X2APIC    = false;
bool CPU::MOVBE     = false;
bool CPU::POPCNT    = false;
bool CPU::TSC       = false;
bool CPU::AESNI     = false;
bool CPU::XSAVE     = false;
bool CPU::OSXSAVE   = false;
bool CPU::AVX       = false;
bool CPU::F16C      = false;
bool CPU::RDRAND    = false;
bool CPU::FPU       = false;
bool CPU::VME       = false;
bool CPU::DE        = false;
bool CPU::PSE       = false;
bool CPU::MSR       = false;
bool CPU::PAE       = false;
bool CPU::MCE       = false;
bool CPU::CX8       = false;
bool CPU::APIC      = false;
bool CPU::SEP       = false;
bool CPU::MTRR      = false;
bool CPU::PGE       = false;
bool CPU::MCA       = false;
bool CPU::CMOV      = false;
bool CPU::PAT       = false;
bool CPU::PSE36     = false;
bool CPU::PSN       = false;
bool CPU::CLFLUSH   = false;
bool CPU::DS        = false;
bool CPU::ACPI      = false;
bool CPU::MMX       = false;
bool CPU::FXSR      = false;
bool CPU::SSE       = false;
bool CPU::SSE2      = false;
bool CPU::SS        = false;
bool CPU::HTT       = false;
bool CPU::TM        = false;
bool CPU::PBE       = false;
bool CPU::Function  = false;
bool CPU::SYSCALL   = false;
bool CPU::XD        = false;
bool CPU::_1GB_PAGE  = false;
bool CPU::RDTSCP    = false;
bool CPU::_64_BIT    = false;



void printf(char* str);

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

        printf("Features:"); //TODO: Add all instructions and features.

        if (edx & EDX_PSE)      { printf(" PSE"); PSE = true;}
        if (edx & EDX_PAE)      { printf(" PAE"); PAE = true;}
        if (edx & EDX_APIC)     { printf(" APIC"); APIC = true;}
        if (edx & EDX_MTRR)     { printf(" MTRR"); MTRR = true;}
 
        printf("\n"); 
 
        printf("Instructions:"); 
 
        if (edx & EDX_TSC)      { printf(" TSC"); TSC = true;}
        if (edx & EDX_MSR)      { printf(" MSR"); MSR = true;}
        if (edx & EDX_SSE)      { printf(" SSE"); SSE = true;}
        if (edx & EDX_SSE2)     { printf(" SSE2"); SSE2 = true;}
        if (ecx & ECX_SSE3)     { printf(" SSE3"); SSE3 = true;}
        if (ecx & ECX_SSSE3)    { printf(" SSSE3"); SSSE3 = true;}
        if (ecx & ECX_SSE41)    { printf(" SSE41"); SSE41 = true;}
        if (ecx & ECX_SSE42)    { printf(" SSE42"); SSE42 = true;}
        if (ecx & ECX_AVX)      { printf(" AVX"); AVX = true;}
        if (edx & EDX_MMX)      { printf(" MMX"); MMX = true; }
        if (ecx & ECX_F16C)     { printf(" F16C"); F16C = true;}
        if (ecx & ECX_RDRAND)   { printf(" RDRAND"); RDRAND = true;}

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