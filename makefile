
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso

GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -std=c++11 -fpermissive
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
		  obj/gdtloader.o \
          obj/gdt.o \
		  obj/memorymanagement.o \
		  obj/multitasking.o \
		  obj/common/memfunc.o \
          obj/drivers/driver.o \
          obj/hardwarecommunication/port.o \
          obj/hardwarecommunication/interruptstubs.o \
          obj/hardwarecommunication/interrupts.o \
          obj/hardwarecommunication/pci.o \
		  obj/hardwarecommunication/cpu.o \
          obj/drivers/keyboard.o \
          obj/drivers/mouse.o \
		  obj/drivers/rtc.o \
		  obj/drivers/vesa/vesa.o \
		  obj/drivers/vesa/int32.o \
		  obj/gui/console.o \
		  obj/gui/color.o \
		  obj/gui/canvas.o \
		  obj/gui/font.o \
		  obj/gui/bmp.o \
		  obj/gui/image.o \
		  obj/gui/shell.o \
          obj/kernel.o


run: mykernel.iso
	#qemu-system-x86_64 -cdrom mykernel.iso
	(killall VirtualBox && sleep 1) || true
	virtualbox --startvm 'OSDev Tests' &

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/drivers/vesa/int32.o: src/drivers/vesa/int32.s #this is nasm syntax
	nasm -f elf $< -o $@

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso

install: mykernel.iso
	cp $< /media/sf_Mint_OSDev/mykernel.iso
	cp mykernel.bin /media/sf_Mint_OSDev/mykernel.bin

.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso
