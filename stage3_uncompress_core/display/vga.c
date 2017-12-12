#include "vga.h"

void display_vga_read_regs(unsigned char *regs)
{
	unsigned i;

    /* read MISCELLANEOUS reg */    
	*regs = IoRead8(VGA_MISC_READ);
	regs++;
    
    /* read SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		IoWrite8(VGA_SEQ_INDEX, i);
		*regs = IoRead8(VGA_SEQ_DATA);
		regs++;
	}
    
    /* read CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		IoWrite8(VGA_CRTC_INDEX, i);
		*regs = IoRead8(VGA_CRTC_DATA);
		regs++;
	}
    
    /* read GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		IoWrite8(VGA_GC_INDEX, i);
		*regs = IoRead8(VGA_GC_DATA);
		regs++;
	}
    
    /* read ATTRIBUTE CONTROLLER regs */
	for (i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)IoRead8(VGA_INSTAT_READ);
		IoWrite8(VGA_AC_INDEX, i);
		*regs = IoRead8(VGA_AC_READ);
brk;        
		regs++;
	}
    
    /* lock 16-color palette and unblank display */
	(void)IoRead8(VGA_INSTAT_READ);
	IoWrite8(VGA_AC_INDEX, 0x20);
}

// Запись регистров в VGA-контроллер
void display_vga_write_regs(unsigned char *regs) {
    
    unsigned i;

    /* write MISCELLANEOUS reg */
	IoWrite8(VGA_MISC_WRITE, *regs);
	regs++;
    
    /* write SEQUENCER regs */
	for (i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		IoWrite8(VGA_SEQ_INDEX, i);
		IoWrite8(VGA_SEQ_DATA, *regs);
		regs++;
	}
    
    /* unlock CRTC registers */
	IoWrite8(VGA_CRTC_INDEX, 0x03);
	IoWrite8(VGA_CRTC_DATA, IoRead8(VGA_CRTC_DATA) | 0x80);

	IoWrite8(VGA_CRTC_INDEX, 0x11);
	IoWrite8(VGA_CRTC_DATA, IoRead8(VGA_CRTC_DATA) & ~0x80);
    
    /* make sure they remain unlocked */
	regs[ 0x03 ] |= 0x80;
	regs[ 0x11 ] &= ~0x80;

    /* write CRTC regs */
	for (i = 0; i < VGA_NUM_CRTC_REGS; i++) {
        
		IoWrite8(VGA_CRTC_INDEX, i);
		IoWrite8(VGA_CRTC_DATA, *regs);
		regs++;
	}
    
    /* write GRAPHICS CONTROLLER regs */
	for (i = 0; i < VGA_NUM_GC_REGS; i++) {
        
		IoWrite8(VGA_GC_INDEX, i);
		IoWrite8(VGA_GC_DATA, *regs);
		regs++;
	}
    
    /* write ATTRIBUTE CONTROLLER regs */
	for (i = 0; i < VGA_NUM_AC_REGS; i++) {
		(void)IoRead8(VGA_INSTAT_READ);
		IoWrite8(VGA_AC_INDEX, i);        
		IoWrite8(VGA_AC_WRITE, *regs);        
		regs++;
	}
    
    /* lock 16-color palette and unblank display */
	(void)IoRead8(VGA_INSTAT_READ);
	IoWrite8(VGA_AC_INDEX, 0x20);    
}

// Писать пиксель
void display_vga_pixel(unsigned x, unsigned y, unsigned char c)
{
    char* video_addr = (char*)0xA0000;

    uint16_t symbol = (x >> 3) + y*80;
    uint16_t mask = 0x8000 >> (x & 7);

    // Установка маски (вертикальная)
	IoWrite16(VGA_GC_INDEX, 0x08 | mask);

    // Читать перед записью, иначе не сработает
    volatile uint8_t t = video_addr[ symbol ];
    video_addr[ symbol ] = c;
}

// Установка определенного видеорежима
void display_vga_mode(int mode) {
    
    switch (mode) {
        
        case VGA_640x480: 
                    
            // -- todo палитру
            display_vga_write_regs( (unsigned char*)disp_vga_640x480x16 );
            IoWrite16(VGA_GC_INDEX, 0x0205);
            break;
        
        case VGA_320x200:
        
            // -- todo палитру
            display_vga_write_regs( (unsigned char*)disp_vga_320x200x256 );
            break;
    }    
}
