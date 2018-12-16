[BITS 32]

[EXTERN main]
[EXTERN pic_keyboard]

[GLOBAL _start]

[GLOBAL apic_disable]
[GLOBAL INT_null]
[GLOBAL IRQ_keyboard]
[GLOBAL IRQ_cascade]

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

INT_null:

        xchg    bx, bx
        iretd

; Обработчик клавиатуры
IRQ_keyboard:

        pushad
        call    pic_keyboard
        mov     al, 20h
        out     20h, al
        popad
        iretd


; Прерывание со slave
IRQ_cascade:

        xchg bx, bx
        pushad
        ; ..
        mov     al, 20h
        out     20h, al
        popad
        iretd
