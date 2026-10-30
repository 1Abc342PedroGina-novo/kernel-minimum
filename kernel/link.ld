/* link.ld - kernel Multiboot i386 */
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
    /* GRUB carrega o kernel em 1 MiB */
    . = 1M;

    /* Header Multiboot PRECISA estar nos primeiros 8 KiB
       e alinhado em 4 bytes. KEEP evita que o linker descarte. */
    .multiboot : ALIGN(4)
    {
        KEEP(*(.multiboot))
    }

    .text : ALIGN(4K)
    {
        *(.text)
    }

    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }

    .data : ALIGN(4K)
    {
        *(.data)
    }

    /* Símbolos para o boot.S zerar .bss (se ele usar) */
    __bss_start = .;
    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
    __bss_end = .;
}
