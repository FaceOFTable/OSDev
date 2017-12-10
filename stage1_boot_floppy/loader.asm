
        org     0
        
        xchg    bx, bx
        
        mov     si, testo
@@:     lodsb
        and     al, al
        je      @f        
        mov     ah, 0Eh
        int     10h
        jmp     @b
        
@@:     jmp     $        
        
testo   db 'testo!' ,0
