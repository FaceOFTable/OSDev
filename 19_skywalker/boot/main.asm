
        org     $7C00
        macro   brk { xchg bx, bx }

        ; 1. Загрузка BIOS $FFFF : 0000
        ; 2. Загрузка 512 байт с MBR (0-й сектор) 
        ; 3. Загрузчик Bootloader загружает в память стартер (kernel)
        ; 4. Стартер запускает ядро и загружает в память ОС
        
        ; В регистре dl будет номер диска BIOS

        mov     ax, bx          ; LOGOTYPE
        cli
        cld
        xor     ax, ax          ; ax = 0
        mov     ss, ax
        mov     sp, $7C00       ; стек под загрузчик
        mov     ds, ax
        mov     es, ax
        mov     [$0000], dl
        
        
