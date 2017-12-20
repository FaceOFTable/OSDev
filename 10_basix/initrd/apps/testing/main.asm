;
; Тестировочный файл
;

        use32
        macro   brk { xchg bx, bx}

        brk
        mov     eax, 40
        push    eax
        pop     ebx
        jmp     $
        
