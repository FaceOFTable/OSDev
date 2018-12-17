
// Хранение данных о прорисовываемых пикселях (300 кб памяти)
uint8_t canvas[ 640*480 ];

// Хранение информации о курсоре для печати символов
struct Tcursor {
    
    // Положение
    int     x, y;
    
    // Цвета и шрифты
    char    frcolor;
    char    bgcolor;        // =-1 Прозрачный
    char    font;           // =0 8x16 Mono; =1 Tahoma 12pt

    // Границы, в которых движется курсор
    int     border_top;
    int     border_right;
    int     border_bottom;
    int     border_left;
    
    // Для печати символов
    int     max_chars;
    
};

struct Tcursor cursor;
