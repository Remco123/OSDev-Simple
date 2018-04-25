.set IRQ_BASE, 0x20

.section .text

.extern _ZN4myos21hardwarecommunication16InterruptManager15HandleInterruptEhj


.macro HandleException num
.global _ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm

.macro HandleExceptionErrCode num
.global _ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global _ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm


HandleException        0x00
HandleException        0x01
HandleException        0x02
HandleException        0x03
HandleException        0x04
HandleException        0x05
HandleException        0x06
HandleException        0x07
HandleExceptionErrCode 0x08
HandleException        0x09
HandleExceptionErrCode 0x0A
HandleExceptionErrCode 0x0B
HandleExceptionErrCode 0x0C
HandleExceptionErrCode 0x0D
HandleExceptionErrCode 0x0E
HandleException        0x0F
HandleException        0x10
HandleExceptionErrCode 0x11
HandleException        0x12
HandleException        0x13

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31

int_bottom:

    # register sichern
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # ring 0 segment register laden
    #mov $0x10, %eax
    #mov %eax, %eds
    #mov %eax, %ees

    # C++ Handler aufrufen
    pushl %esp
    push (interruptnumber)
    cld
    call _ZN4myos21hardwarecommunication16InterruptManager15HandleInterruptEhj
    mov %eax, %esp # den stack wechseln

    # register laden
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $4, %esp             # Remove error code
.global _ZN4myos21hardwarecommunication16InterruptManager15InterruptIgnoreEv
_ZN4myos21hardwarecommunication16InterruptManager15InterruptIgnoreEv:

    iret


.data
    interruptnumber: .byte 0