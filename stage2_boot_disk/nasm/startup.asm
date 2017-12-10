[BITS 32]

[EXTERN main]
[GLOBAL _start]

_start:

        mov     esp, 0xA0000
        jmp     main
