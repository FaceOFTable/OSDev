
        org     8000h
        macro   brk {  xchg    bx, bx }
        
        ; Загрузка регистра GDT/IDT
        cli
        
        lgdt    [GDTR]      
        lidt    [IDTR] 

        ; Из FAT32 грузится прямо в XMS-память
        call    initrd
        
        ; Переход в текстовый режим
        mov     ax, 0003h
        int     10h

        ; Вход в Protected Mode
        mov     eax, cr0
        or      al, 1
        mov     cr0, eax
        jmp     10h : pm
        
        include "asm/initrd.asm"
        
; ----------------------------------------------------------------------
GDTR:   dw 6*8 - 1                  ; Лимит GDT (размер - 1)
        dq GDT                      ; Линейный адрес GDT 
IDTR:   dw 256*8 - 1                ; Лимит GDT (размер - 1)
        dq 0                        ; Линейный адрес GDT          
GDT:    dw 0,      0,    0,     0        ; 00 NULL-дескриптор
        dw 0FFFFh, 0,    9200h, 00CFh    ; 08 32-bit данные
        dw 0FFFFh, 0,    9A00h, 00CFh    ; 10 32-bit код
        dw 103h,   800h, 8900h, 0040h    ; 18 32-bit главный TSS
        dw 0FFFFh, 0,    9200h, 008Fh    ; 20 16-битный дескриптор данных
        dw 0FFFFh, 0,    9A00h, 008Fh    ; 28 16-bit код
 
; ----------------------------------------------------------------------

        use32
pm:     mov     ax, 8
        mov     ds, ax
        mov     es, ax
        mov     ss, ax
        mov     fs, ax
        mov     gs, ax
        mov     ax, 18h
        ltr     ax
        
        ; Распаковка initrd
        ; ...
        
        
        ; Скопировать ядро в память
        mov     esi, os
        mov     edi, 100000h
        mov     ecx, len
        rep     movsb
        
        jmp     100000h
        
os:     file    "kernel.c.bin"
len =   $ - os
        
