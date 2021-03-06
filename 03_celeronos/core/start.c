// Процедуры, которые выполняются при запуске ядра, инциализация системы
void startup() {
	
	cls(1);

	// Простановка основных прерываний
	// -------------------------------------------------
    sys_irq_redirect(IRQ_KEYB | IRQ_TIMER);
    sys_irq_make();

    sys_irq_create(0x20, (u32*) &isr_timer);
    sys_irq_create(0x21, (u32*) &isr_keyboard);
    sys_irq_create(0x22, (u32*) &isr_cascade);
	sys_irq_create(0x2C, (u32*) &isr_ps2mouse);
    
    // Включение страничного механизма
	// -------------------------------------------------
    enable_paging();
    memory_create_new_gdt();

    // Инициализация устройств
	// -------------------------------------------------
    sys_timer_init();
	
	// Инициализация стандартных цветов
	// -------------------------------------------------
	dac_set( 0, 0x000000); // Чёрный
	dac_set( 1, 0x000080); // Синий
	dac_set( 2, 0x008000); // Зелёный
	dac_set( 3, 0x008080); // Голубой
	dac_set( 4, 0x800000); // Красный
	dac_set( 5, 0x800080); // Сиреневый
	dac_set( 6, 0x808000); // Коричневый
	dac_set( 7, 0xC0C0C0); // Серый
	dac_set( 8, 0x808080); // Тёмно-серый
	dac_set( 9, 0x0000FF); // Светло-синий
	dac_set(10, 0x00FF00); // Светло-зелёный
	dac_set(11, 0x00FFFF); // Светло-голубой
	dac_set(12, 0xFF0000); // Ярко-красный
	dac_set(13, 0xFF00FF); // Пурпурный
	dac_set(14, 0xFFFF00); // Жёлтый
	dac_set(15, 0xFFFFFF); // Белый
	
}