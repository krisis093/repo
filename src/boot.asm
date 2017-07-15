global loader

MAGICNUMBER equ 0x1BADB002;
KERNELSTACKSIZE equ 4096;

section .text
align 4
	dd MAGICNUMBER
	dd 0x0;multiboot flag
	dd -MAGICNUMBER

loader:
align 4
	mov esp, kernel_stack + KERNELSTACKSIZE
	extern kernel_main
	call kernel_main

section .bss
kernel_stack:
align 4
	resb KERNELSTACKSIZE ; prerequisite for C call stack

