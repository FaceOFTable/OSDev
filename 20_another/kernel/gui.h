#define WINDOW_MAX  64      // Максимум окон

struct window {

    int     x1, y1;
    int     x2, y2;
    int     w, h;
    char*   title;
    uint8_t in_use;         // Это окно загружено в систему
    uint8_t active;         // Окно активно
    uint8_t bgcolor;        // Окно активно
};

// Зарегистрированные в системе окна
struct window allwin[ WINDOW_MAX ];

// Количество окон
int    window_count;
