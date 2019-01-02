int miner;

void miner_repaint() {

    int i, j;
    
    gdi_fillrect(miner, 0, 0, 164, 3, 15);
    gdi_fillrect(miner, 0, 0, 3, 208, 15);
    
    gdi_ridge_rect(miner, 10, 50, 147, 147, 8, 15, 2);
    gdi_ridge_rect(miner, 10, 10, 147, 30, 8, 15, 2);
    
    for (i = 0; i < 9; i++)
    for (j = 0; j < 9; j++) {
        gdi_ridge_rect(miner, 12 + j*16, 52 + i*16, 15, 15, 15, 8, 2);    
    }
}

// Игрулька
void make_miner() {

    miner = window_create(64, 64, 164, 208, "Сапер");

    // Регистрация событий
    window_event(miner, EVENT_REPAINT, & miner_repaint);
    
    // Активация нового окна и перерисовка
    window_activate(miner); 
    window_repaint(miner);
}
