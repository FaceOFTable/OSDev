#define brk __asm__ __volatile__("xchg %bx, %bx");
#define sti __asm__ __volatile__("sti");
#define cli __asm__ __volatile__("cli");

// Объявления
// ---------------------------------------------------------------------
void apic_disable();            // Отключить LAPIC
void INT_null();                // Заглушка INT
void IRQ_keyboard();
void IRQ_cascade();

// Некоторые константы
// ---------------------------------------------------------------------

#define DISPLAY_TEXT_ADDR (char*)0xB8000

// Типы данных
// ---------------------------------------------------------------------
// См. http://ru.cppreference.com/w/cpp/language/types

#define int8_t      signed char
#define uint8_t     unsigned char

#define int16_t     signed short
#define uint16_t    unsigned short

#define int32_t     signed int
#define uint32_t    unsigned int

#define int64_t     long long
#define uint64_t    unsigned long long

#define size_t      unsigned int

// 64-х битный дескриптор прерывания
struct IDT_Item {

    uint16_t low_addr;
    uint16_t selector;
    uint16_t attr;
    uint16_t hi_addr;
};

// Адреса PIC 8086/88
// ---------------------------------------------------------------------

#define PIC1             0x20  /* IO базовый адрес для master PIC */
#define PIC2             0xA0  /* IO базовый адрес для slave PIC */

#define PIC1_COMMAND     PIC1
#define PIC1_DATA        (PIC1+1)

#define PIC2_COMMAND     PIC2
#define PIC2_DATA        (PIC2+1)

#define PIC_EOI          0x20  /* End-of-interrupt command code */

#define ICW1_ICW4        0x01  /* ICW4 (not) needed */
#define ICW1_SINGLE      0x02  /* Single (cascade) mode */
#define ICW1_INTERVAL4   0x04  /* Call address interval 4 (8) */
#define ICW1_LEVEL       0x08  /* Level triggered (edge) mode */
#define ICW1_INIT        0x10  /* Initialization - required! */

#define ICW4_8086        0x01  /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO        0x02  /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE   0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER  0x0C  /* Buffered mode/master */
#define ICW4_SFNM        0x10  /* Special fully nested (not) */

// Маски
#define IRQ_TIMER        (1 << 0)
#define IRQ_KEYB         (1 << 1)
#define IRQ_CASCADE      (1 << 2)
#define IRQ_PS2MOUSE     (1 << 12)

// Процедура необходимой задержки для корректной работы прерываний
#define IoWait asm volatile("jecxz 1f" "\n\t" "1:jecxz 2f" "\n\t" "2:");

// Писать в (port) данные data
// ---------------------------------------------------------------------

static inline void IoWrite8(int16_t port, int8_t data) {
    __asm__ volatile("outb %b0, %w1" : : "a" (data), "Nd" (port));
}

static inline void IoWrite16(int16_t port, int16_t data) {
    __asm__ volatile("outw %w0, %w1" : : "a" (data), "Nd" (port));
}

static inline void IoWrite32(int16_t port, int32_t data) {
    __asm__ volatile("outl %0, %w1" : : "a" (data), "Nd" (port));
}

// Читать данные из порта (port)
// ---------------------------------------------------------------------
static inline uint8_t IoRead8(int16_t port) {

    uint8_t data;
    __asm__ volatile("inb %1, %0" : "=a" (data) :"Nd" (port));
    return data;
}

static inline uint16_t IoRead16(int16_t port) {

    uint16_t data;
    __asm__ volatile("inw %1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline uint32_t IoRead32(int16_t port) {

    uint32_t data;
    __asm__ volatile("inl %1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

// ---------------------------------------------------------------------
