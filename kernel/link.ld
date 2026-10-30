// kernel.c - Kernel mínimo
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile unsigned short*)0xB8000)

static unsigned int cursor_x = 0;
static unsigned int cursor_y = 0;
static unsigned char color = 0x0F; // branco em fundo preto

static unsigned short vga_entry(char c) {
    return (unsigned short)c | ((unsigned short)color << 8);
}

void clear_screen(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        VGA_MEMORY[i] = vga_entry(' ');
    cursor_x = cursor_y = 0;
}

void putchar(char c) {
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
    if (cursor_y >= VGA_HEIGHT) {
        clear_screen();
    }
}

void print(const char *s) {
    while (*s) putchar(*s++);
}

// Protótipo da função chamada pelo boot.asm
void kmain(unsigned int magic, unsigned int *mbinfo) {
    (void)magic;
    (void)mbinfo;

    clear_screen();
    print("=== Kernel Minimo de Teste ===\n");
    print("Boot via Multiboot OK!\n");
    print("Se voce esta lendo isso, o kernel rodou.\n");

    // loop infinito
    while (1) {
        __asm__ volatile ("hlt");
    }
}
