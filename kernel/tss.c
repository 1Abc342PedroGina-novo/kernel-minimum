// tss.c
#include "tss.h"
#include "gdt.h"

static struct tss_entry tss;
extern struct gdt_entry gdt[];   // de gdt.c

void tss_init(uint32_t kernel_stack) {
    uint8_t *p = (uint8_t *)&tss;
    for (unsigned i = 0; i < sizeof(tss); i++) p[i] = 0;

    tss.ss0  = 0x10;                 // kernel data (index 2 << 3)
    tss.esp0 = kernel_stack;         // stack do kernel usada em ring 3 -> 0
    tss.iomap_base = sizeof(tss);    // sem I/O bitmap (bloqueia I/O em ring 3)
}

void tss_set_kernel_stack(uint32_t stack) {
    tss.esp0 = stack;
}

void tss_set_gdt_entry(int index) {
    uint32_t base  = (uint32_t)&tss;
    uint32_t limit = sizeof(tss) - 1;

    gdt[index].base_low    = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high   = (base >> 24) & 0xFF;
    gdt[index].limit_low   = (limit & 0xFFFF);
    gdt[index].granularity = ((limit >> 16) & 0x0F) | 0x00;
    gdt[index].access      = 0x89;   // present, ring 0, TSS 32-bit available
}
