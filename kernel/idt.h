// idt.h
#ifndef IDT_H
#define IDT_H

#include "gdt.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_init(void);
void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags);
void isr_handler_c(uint32_t int_no, uint32_t err_code);

#endif
