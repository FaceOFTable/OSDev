        
        
        use16
        org     $8000
        macro   brk { xchg bx, bx }
        
        ; Вход в защищенный режим        
        include "core/enter_protected_mode.asm"    
        use32
        
        ; Функции уже защищенного режима
        include "core/pic_irq_redirect.asm"    
            
protected_mode_entry:

        ; Инициализация сегментов и многозадачности
        mov     ax, $10
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax
        mov     ax, $18
        mov     esp, $8000
        ltr     ax
        
        brk
        call    pic_irq_redirect
        
        
        jmp     $
        
