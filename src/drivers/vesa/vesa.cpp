#include <drivers/vesa/vesa.h>

#define DIFF(x, y) ((x) - (y))
typedef __UINTPTR_TYPE__ uintptr_t;

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

void printf(char*);
void printfHex16(uint16_t);
void printfHex32(uint32_t);

Vesa::Vesa()
{
    this->FrameBufferAddr = 0;
}


void set_vesa_mode(uint16_t mode) {
	regs16_t regs;
	regs.ax = 0x4f02;
	regs.bx = mode;
	int32(0x10,&regs);
}

uint16_t findMode(int x, int y, int d)
{
	uint16_t best = 0;

	int pixdiff, bestpixdiff = DIFF(320 * 200, x * y);
	int depthdiff, bestdepthdiff = 8 >= d ? 8 - d : (d - 8) * 2;

    regs16_t regs;
	VESA_INFO* ctrl = (VESA_INFO*) 0x2000;
	MODE_INFO* inf = (MODE_INFO*) 0x3000;

    MemoryFunctions::memcpy(ctrl->signature, "VBE2", 4);

	regs.ax = 0x4F00;
	regs.es = 0;
	regs.di = (uintptr_t) ctrl;
	int32(0x10, &regs);

	uint16_t* modes = (uint16_t*) ctrl->video_modes;
	for(int i = 0; modes[i] != 0xFFFF; i++)
	{
		regs.ax = 0x4F01;
		regs.cx = modes[i];
		regs.di = (uintptr_t) inf;
		int32(0x10, &regs);

		if ((uint16_t) regs.ax != 0x004F) continue;

		printf("Found Mode: 0x"); printfHex16(modes[i]); printf("\n");

        if (x == inf->resolutionX && y == inf->resolutionY && d == inf->bpp)
			set_vesa_mode(modes[i]);
    }
    set_vesa_mode(best);
}

void Vesa::SelectMode(Canvas* canv)
{
    set_vesa_mode(0x144 | 0x4000);

	Port8Bit::Write8(0xA0, 0x20);
    Port8Bit::Write8(0x20, 0x20);
}