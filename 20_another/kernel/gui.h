#define WINDOW_MAX  64      // Максимум окон

struct window {

    uint8_t in_use;         // Это окно загружено в систему?
    uint8_t active;         // Окно активно (=1)

    // Размеры окна и внешний вид
    int     x1, y1;
    int     x2, y2;
    int     w, h;
    char*   title;
    uint8_t bgcolor;

    // События, которые может обрабатывать окно
    void    (*event_keypress)();
    void    (*event_keyup)();
    void    (*event_mousedown)();
    void    (*event_mouseup)();
    void    (*event_mousemove)();
    void    (*event_mouseout)();
    void    (*event_mousein)();
    void    (*event_close)();
    void    (*event_repaint)();
};

// Зарегистрированные в системе окна
struct window allwin[ WINDOW_MAX ];

// Количество окон
int    window_count;
