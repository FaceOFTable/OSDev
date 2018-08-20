        
        
        use16
        org     $8000
        macro   brk { xchg bx, bx }
        
        ; Вход в защищенный режим        
        include "core/enter_protected_mode.asm"    
        use32
            
protected_mode_entry:

        ; Инициализация сегментов и многозадачности
        mov     ax, $10
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax
        mov     ax, $18
        ltr     ax
        
        brk
        jmp     $
        
