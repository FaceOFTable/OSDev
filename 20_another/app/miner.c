int miner_hwnd;

// Игрулька
void make_miner() {

    miner_hwnd = window_create(32, 32, 256, 256, "Сапер");
    window_activate(miner_hwnd); 
    window_repaint(miner_hwnd);
}
