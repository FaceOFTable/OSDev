[BITS 32]

[EXTERN main]
[EXTERN kernel_pic_keyb]
[GLOBAL _start]
[GLOBAL apic_disable]
[GLOBAL interrupt_null]
[GLOBAL interrupt_keyb]

; ----------------------------------------------------------------------
_start:

        mov     esp, 0xA0000
        jmp     main

; Отключение локального APIC
; ----------------------------------------------------------------------
apic_disable:

        mov ecx, 0x1b
        rdmsr
        and eax, 0xfffff7ff
        wrmsr
        ret
     
; ----------------------------------------------------------------------
; ПРЕРЫВАНИЯ

interrupt_null:

        xchg    bx, bx
        iretd

; Обработчик клавиатуры
interrupt_keyb:

        pushad        
        call    kernel_pic_keyb        
        mov     al, 20h
        out     20h, al
        popad
        iretd
