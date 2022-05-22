#ifndef LCDFinal_H_
#define LCDFinal_H_




#define lcd_Clear            0x01          // replace all characters with ASCII 'space'
#define lcd_Home             0x02          // return cursor to first position on first line
#define lcd_Cursor_Right     0x06          // shift cursor from left to right on read/write
#define lcd_DisplayOff       0x08          // turn display off
#define lcd_DisplayOn        0x0F          // display on
#define lcd_FunctionReset    0x30          // reset the LCD
#define lcd_FunctionSet8bit  0x38          // 8-bit data, 2-line display, 5 x 7 font
#define first_line1          0x80          // set cursor position in first row
#define first_line2          0xC0          // set cursor position in second row



//Interface to send the configuration commands to the LCD Driver
void lcd_cmd(unsigned char cmd);

//Apply initialization sequence for LCD module
void init_lcd(void);   

//Interface to write character on LCD screen        
void lcd_write(unsigned char data);

//Interface to write string on LCD screen
void lcd_write_string(); 

#endif