;
; Определение максимального размера памяти
;

mm.memory_top   dd  0

; Максимальный размер памяти находится в EAX
; То что находится там, означает, что начиная с этого адреса, физическая
; память более недоступна

mm.MemSize:

        xor     eax, eax    
.ok:    add     eax, 0x00100000         ; Начиная с 1-го мегабайта
        cmp     eax, 0xE0000000
        je      .done
        mov     ebx, [eax]              ; Взять значение
        xor     [eax], dword 0x5A5A5A5A ; Изменить то что было
        cmp     [eax], ebx              ; Проверить на изменения
        mov     [eax], ebx              ; Записать значение обратно
        jne     .ok
.done:  mov     [mm.memory_top], eax    ; Скопировать данные сюда
        ret
