; gdt_flush.S
global gdt_flush
global tss_flush

section .text
gdt_flush:
    mov eax, [esp + 4]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush      ; far jump pra recarregar CS
.flush:
    ret

tss_flush:
    mov ax, 0x28         ; seletor do TSS = index 5 << 3
    ltr ax
    ret
