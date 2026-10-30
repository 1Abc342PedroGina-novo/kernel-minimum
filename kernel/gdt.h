// gdt.h
#ifndef GDT_H
#define GDT_H

typedef unsigned int  uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned char  uint8_t;
typedef unsinged long  uint64_t;

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void gdt_init(void);

#endif
