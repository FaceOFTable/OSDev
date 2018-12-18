// Очистка регионов памяти
void kernel_init() {

    int i;

    // Назначить обработчики
    irq_init(IRQ_KEYB);

    // Инициализация окон
    window_count = 0;

    for (i = 0; i < WINDOW_MAX; i++) {
        allwin[i].in_use = 0;
        allwin[i].active = 0;
    }

    // Определить размер памяти
    memory_size();
    
    // Установить позицию мыши
    set_mouse_xy(320, 240);    
    cursor.mouse_show = 1;
}

// Конвертация числа в ASCIIZ
int i2a(int num, char* res) {

    char fm[24];

    int i, j = 0, k = 0, neg = 0, dignum = 0;

    // Пропечатка отрицательного числа
    if (num < 0) {
        num = -num;
        dignum++;
        neg = 1;
    }

    // Ищем данные
    for (i = 0; i < 10; i++) {

        dignum++;
        fm[j++] = ('0' + num % 10);
        num = num / 10;

        if (num == 0)
            break;
    }
    
    // Отрицательный знак
    if (neg) res[k++] = '-';

    // Переписываем в обратную сторону
    for (j--; j >= 0; j--) res[k++] = fm[j]; res[k] = 0;

    return dignum;
}
