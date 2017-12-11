
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
LZW_table       EQU 100000h
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

        ; Распаковка LZW
        mov     ebx, 200003h     ; Данные для распаковки в 2mb
        mov     edi, [start_xms] ; Куда распаковать
        mov	    [LZW_bits], 0    
        
LZW_clear:  ; Очистка словаря

        xor	    edx, edx
    
LZW_decompress_loop:

        ; В зависимости от размера словаря, будет использованы CH бит
        mov     eax, [initrd_size]
        add     eax, 200000h
        cmp     ebx, eax
        jnb     LZW_end
        
        ; 200h (-100h)
        mov	    ch, 9
        cmp	    edx, (0100h - 1)*8
        jbe	    LZW_read_bits
        
        ; 400h (-100h)
        mov	    ch, 10
        cmp	    edx, (0300h - 1)*8
        jbe	    LZW_read_bits
        
        ; 800h (-100h)
        mov	    ch, 11
        cmp	    edx, (0700h - 1)*8
        jbe	    LZW_read_bits
        
        ; 1000h (-100h)
        mov	    ch, 12
        cmp	    edx, (0F00h - 1)*8
        jbe	    LZW_read_bits

        ; 2000h (-100h)
        mov	    ch, 13        
        cmp	    edx, (1F00h - 1)*8
        jbe	    LZW_read_bits
        
        ; 4000h (-100h)
        mov	    ch, 14        
        cmp	    edx, (3F00h - 1)*8
        jbe	    LZW_read_bits
        
        ; 8000h (-100h)
        mov	    ch, 15        
        cmp	    edx, (7F00h - 1)*8
        jbe	    LZW_read_bits
        
        ; 10000h
        mov	    ch, 16
    
LZW_read_bits:

        ; Сдвинуть на CL бит 
        mov	    cl, [LZW_bits]
        mov	    eax, [ebx]
        shr	    eax, cl
        
        ; Срезать CH битов
        ; CL -> указатель на следующие биты
        xchg	cl, ch
        mov	    esi, 1
        shl	    esi, cl
        dec	    esi
        and	    eax, esi            
        add	    cl, ch      
    
LZW_read_bits_count:

        cmp	    cl, 8
        jbe	    LZW_read_bits_ok
        
        ; Обнаружено превышение байта, передвинуть указатель потока +8 бит
        ; до тех пор, пока CL не будет <= 8

        sub	    cl, 8
        inc	    ebx
        jmp	    LZW_read_bits_count
    
LZW_read_bits_ok:

        mov	    [LZW_bits], cl
        cmp	    eax, 100h
        jb	    LZW_single_byte         ; ax < 100h -- простой байт
        je	    LZW_cmd                 ; ax = 100h -- команда очистки словаря

        sub     eax, 101h               ; Использовать указатель на построенный словарь
        shl	    eax, 3
        cmp	    eax, edx
        ja	    LZW_error               ; eax - указатель на словарь если edx < eax, словарь превышен
        
        ; СЛОВАРЬ (8 байт на 1 эл-т)
        
        ; 4 | +0 | Количество символов
        ; 4 | +4 | Указатель на строку для повторения
        
        mov	    ecx, [LZW_table + eax]
        mov	    esi, [LZW_table + eax + 4]
        
        ; Записать в следующий элемент текущий указатель EDI (построение словаря)
        mov	    [LZW_table + edx + 4], edi  
        rep	    movsb
        
        ; Скопировать кол-во символов из предыдущего элемента и +1 к длине
        mov	    eax, [LZW_table + eax]
        inc	    eax
        mov	    [LZW_table + edx], eax
        jmp	    LZW_decompress_next
        
        ; Строительство нового словаря
    
LZW_single_byte:

        mov	    [LZW_table + edx], dword 2  ; Добавить словарь: длина = 2, указатель
        mov	    [LZW_table + edx + 4], edi  ; текущий указатель на выходной поток
        stosb                               ; Скопировать один байт из входящего потока
        
        ; Добавляем +1 эл-т к словарю и переходим далее
    
LZW_decompress_next:

        add	    edx, 8
        jmp	    LZW_decompress_loop

; ----------------------------------------------------------------------
; Неизвестно, что за команда, но пусть будет дебаг на нее. Видимо, это
; очистка словаря, когда тот > 64кб

LZW_cmd:

        brk
        xchg    ax, ax
        jmp     $

; Ошибка декомпрессии
LZW_error:

        brk
        xchg    cx, cx
        jmp     $

; ----------------------------------------------------------------------            
LZW_bits        db 0        
; ----------------------------------------------------------------------            
LZW_end:
    
        ; Скопировать ядро в память
        mov     esi, os
        mov     edi, 100000h
        mov     ecx, len
        rep     movsb
        
        jmp     100000h
        
os:     file    "kernel.c.bin"
len =   $ - os
        
