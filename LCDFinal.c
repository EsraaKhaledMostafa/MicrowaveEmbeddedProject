#include "STDFinal.h"
#include "GPIOFinal.h"
#include "tm4c123gh6pm.h"
#include "LCDFinal.h"

//Apply initialization sequence for LCD module
void init_lcd (void)                     
{
	/* Function Set  */
	lcd_cmd(lcd_FunctionSet8bit);
	genericDelay(1);

	
	/* Entry Mode Set  */
	lcd_cmd(lcd_Cursor_Right);
	genericDelay(1);
	
	/* Display ON OFF Control */
	lcd_cmd(lcd_DisplayOn);
	genericDelay(1);
	
	/* Clear Display */
	lcd_cmd(lcd_Clear);
	genericDelay(2);
	
	/* Return Home  */
	lcd_cmd(lcd_Home);
	genericDelay(2);
	
	return;
}



//Interface to send the configuration commands to the LCD Driver
void lcd_cmd(unsigned char cmd)            
{
	GPIO_PORTB_DATA_R=cmd;
	
	GPIO_PORTE_DATA_R=(LOW<<PIN1)|(LOW<<PIN2)|(HIGH<<PIN3);
	genericDelay(1);
	GPIO_PORTE_DATA_R=(LOW<<PIN1)|(LOW<<PIN2)|(LOW<<PIN3);
	genericDelay(50);
	
	return;
}




//Interface to write character on LCD screen
void lcd_write(unsigned char data)
{
	GPIO_PORTB_DATA_R=data;
	
	GPIO_PORTE_DATA_R=(HIGH<<PIN1)|(LOW<<PIN2)|(HIGH<<PIN3);
	genericDelay(1);
	GPIO_PORTE_DATA_R=(HIGH<<PIN1)|(LOW<<PIN2)|(LOW<<PIN3);
	genericDelay(50);
	
	return;
}



//Interface to write string on LCD screen
void lcd_write_string(unsigned char *str)  
{
	int		i=0;
	while(str[i]!='\0')
	{
		lcd_write(str[i]);
		i++;
		
	}
	
	genericDelay(100);
	
	return;
	
}





