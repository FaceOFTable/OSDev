#define WINDOW_MAX  64      // Максимум окон

struct window {

    int     x1, y1;
    int     x2, y2;
    int     w, h;
    char*   title;
    int     in_use;         // Это окно загружено в систему
    int     active;         // Окно активно
    int     icon;           // Иконка. Пока не важно
};

// Зарегистрированные в системе окна
struct window allwin[ WINDOW_MAX ];

// Количество окон
int    window_count;
