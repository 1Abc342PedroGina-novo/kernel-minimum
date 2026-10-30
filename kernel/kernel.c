// kernel.c - Kernel mínimo com saída VGA + serial
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile unsigned short*)0xB8000)

#define COM1 0x3F8

static unsigned int cursor_x = 0;
static unsigned int cursor_y = 0;
static unsigned char color = 0x0F;

/* ---------- I/O de portas ---------- */
static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char r;
    __asm__ volatile ("inb %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}

/* ---------- Serial (COM1) ---------- */
static void serial_init(void) {
    outb(COM1 + 1, 0x00); // desabilita interrupções
    outb(COM1 + 3, 0x80); // habilita DLAB
    outb(COM1 + 0, 0x03); // divisor low  (38400 baud)
    outb(COM1 + 1, 0x00); // divisor high
    outb(COM1 + 3, 0x03); // 8N1
    outb(COM1 + 2, 0xC7); // FIFO
    outb(COM1 + 4, 0x0B); // IRQs + RTS/DSR
}

static void serial_putchar(char c) {
    while (!(inb(COM1 + 5) & 0x20)) { } // espera THR vazio
    outb(COM1, (unsigned char)c);
}

static void serial_print(const char *s) {
    while (*s) serial_putchar(*s++);
}

/* ---------- VGA ---------- */
static unsigned short vga_entry(char c) {
    return (unsigned short)c | ((unsigned short)color << 8);
}

static void clear_screen(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        VGA_MEMORY[i] = vga_entry(' ');
    cursor_x = cursor_y = 0;
}

static void vga_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c);
        if (++cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    if (cursor_y >= VGA_HEIGHT) clear_screen();
}

/* ---------- Saída unificada ---------- */
static void putchar(char c) {
    vga_putchar(c);
    // converte \n em \r\n na serial (terminais UNIX)
    if (c == '\n') serial_putchar('\r');
    serial_putchar(c);
}

static void print(const char *s) {
    while (*s) putchar(*s++);
}

/* ---------- Entrada ---------- */
void kmain(unsigned int magic, unsigned int *mbinfo) {
    (void)magic;
    (void)mbinfo;

    serial_init();
    clear_screen();

    print("=== Kernel Minimo de Teste ===\n");
    print("Boot via Multiboot OK!\n");
    print("Se voce esta lendo isso, o kernel rodou.\n");
    print("KERNEL_OK\n");   // marcador estável pro CI

    while (1) {
        __asm__ volatile ("hlt");
    }
}
