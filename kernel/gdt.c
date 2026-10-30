// gdt.c
#include "gdt.h"
#include "tss.h"

#define GDT_ENTRIES 6

static struct gdt_entry gdt[GDT_ENTRIES];
static struct gdt_ptr   gp;

extern void gdt_flush(uint32_t);   // definido em gdt_flush.S (ou inline asm abaixo)
extern void tss_flush(void);

static void gdt_set_gate(int i, uint32_t base, uint32_t limit,
                         uint8_t access, uint8_t gran) {
    gdt[i].base_low    = (base & 0xFFFF);
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;

    gdt[i].limit_low   = (limit & 0xFFFF);
    gdt[i].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);

    gdt[i].access      = access;
}

void gdt_init(void) {
    gp.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gp.base  = (uint32_t)&gdt;

    // 0: null
    gdt_set_gate(0, 0, 0, 0, 0);
    // 1: kernel code  (base=0, limit=4GB, access=0x9A, gran=0xCF)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // 2: kernel data
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // 3: user code
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // 4: user data
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    // 5: TSS (preenchido por tss_init)
    tss_set_gdt_entry(5);

    // carrega GDT e recarrega segmentos
    __asm__ volatile ("lgdt (%0)" : : "r"(&gp));
    gdt_flush(0x08);        // 0x08 = seletor do kernel code (index 1 << 3)
    tss_flush();            // ltr 0x28 (index 5 << 3 | RPL 0)
}
