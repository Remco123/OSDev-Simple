#ifndef MYOS__VESA__REALMODE_H
#define MYOS__VESA__REALMODE_H

struct regs16_t {
    unsigned short di, si, bp, sp, bx, dx, cx, ax;
    unsigned short gs, fs, es, ds, eflags;
} __attribute__ ((packed));

extern "C" void int32(unsigned char intnum, regs16_t *regs);

#endif