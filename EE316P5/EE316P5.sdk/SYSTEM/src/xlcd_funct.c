#include "main.h"

#define RS 0x20     /* PB5 mask for reg select */
#define EN 0x80     /* PB7 mask for enable */



/* initialize GPIOB/C then initialize LCD controller */
void LCD_init(void) {

    usleep(20*1000);                /* LCD controller reset sequence */
    LCD_nibble_write(0x30, 0);
    usleep(5*1000);
    LCD_nibble_write(0x30, 0);
    usleep(1*1000);
    LCD_nibble_write(0x30, 0);
    usleep(1*1000);

    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    usleep(1*1000);
    LCD_command(0x28);          /* set 4-bit data, 2-line, 5x7 font */
    usleep(100);
    LCD_command(0x06);          /* move cursor right */
    usleep(100);
    LCD_command(0x01);          /* clear screen, move cursor to home */
    usleep(100);
    LCD_command(0x0F);          /* turn on display, cursor blinking */
}


void LCD_nibble_write(char data, unsigned char control) {
	//data &= 0xF0;
	//control &= 0x0F;
    /* populate data bits */
	Xil_Out32(LCD_GPIO, data | control);
	usleep(0);
	Xil_Out32(LCD_GPIO, data | control | EN);
	usleep(0);
	Xil_Out32(LCD_GPIO, data | control);
	usleep(0);
	//Xil_Out32(LCD_GPIO, 0);

}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command >>4 , 0);    /* upper nibble first */
    LCD_nibble_write(command & 0xF, 0);      /* then lower nibble */

    if (command < 4)
        usleep(4*1000);         /* command 1 and 2 needs up to 1.64ms */
    else
        usleep(1*1000);         /* all others 40 us */
}

void LCD_data(char data) {
    LCD_nibble_write(data >>4 , RS);      /* upper nibble first */
    LCD_nibble_write(data & 0xF, RS);        /* then lower nibble */

    usleep(1*1000);
}

void LCD_data_ln(char* string){
	while(*string){
		LCD_data(*string++ & 0xFF);
	}

}
