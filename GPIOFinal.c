#include "GPIOFinal.h"
#include "tm4c123gh6pm.h"
#include <Math.h>
#include <string.h> // For strtok function
#include <stdlib.h> // For strtod function
#include <stdint.h>
#include <stdio.h>
#include "LCDFinal.h"


// Systick functions 

#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))

	void SysTick_wait(unsigned long delay)
	{
		NVIC_ST_CTRL_R=0x00;
		NVIC_ST_RELOAD_R= delay-1;
		NVIC_ST_CURRENT_R=0;
		NVIC_ST_CTRL_R=0x05;
	
		while((NVIC_ST_CTRL_R& 0x00010000)==0) { }   //wait for COUNT Flag
	}

	void genericDelay(unsigned long time)
{	
	int i;
	for(i=0;i< time ;i++)
	{
		SysTick_wait(16000);              //wait 1ms
	}
}


/* IO Ports Initialization Functions Implementations*/
void PortE_Init(void)
{ // for RS , RW , E pins (1,2,3) 
	SYSCTL_RCGCGPIO_R |= 0x00000010;
  while ((SYSCTL_PRGPIO_R&0x10) == 0){};
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_CR_R |= 0x3E;
	GPIO_PORTE_DIR_R = 0x0E;
	GPIO_PORTE_DEN_R = 0x3E;
	GPIO_PORTE_AMSEL_R = 0x00;
	GPIO_PORTE_AFSEL_R = 0x30;
	GPIO_PORTE_PCTL_R = 0x00110000;
  GPIO_PORTE_PUR_R = 0x00;
}

void PortB_Init ()
{ // for 3-Seven segment display or LCD (BONUS)
	SYSCTL_RCGCGPIO_R |= 0x00000002;
  while ((SYSCTL_PRGPIO_R&0x02) == 0){};
	GPIO_PORTB_LOCK_R = 0x4C4F434B;
	GPIO_PORTB_CR_R |= 0xFF;
	GPIO_PORTB_DIR_R = 0xFF;
	GPIO_PORTB_DEN_R = 0xFF;
	GPIO_PORTB_AMSEL_R = 0x00;
	GPIO_PORTB_AFSEL_R = 0x00; 
	GPIO_PORTB_PCTL_R = 0x00000000;
  GPIO_PORTB_PUR_R = 0x00;
  
}
void Port_AC_Init(){

 SYSCTL_RCGCGPIO_R |= 0x00000005;        //Enable clock for PORTA and PORTB  
  //while ((SYSCTL_PRGPIO_R&0x03) == 0){};  //Wait for clock to be setted
	GPIO_PORTA_LOCK_R = 0x4C4F434B;
	GPIO_PORTC_LOCK_R = 0x4C4F434B;
  GPIO_PORTC_CR_R |= 0xF0;             //Allow settings for pins (4,5,6,7) of PORTC
  GPIO_PORTA_CR_R |= 0x3C;             //Allow settings for pins (2,3,4,5) of PORTA
	GPIO_PORTC_DIR_R |= 0xF0;             //PC4-PC7 are used with col and set them as digital output pins
  GPIO_PORTA_DIR_R = 0x00;             //PA2-PA5 are used with row and set them as digital input pins
  GPIO_PORTA_PDR_R = 0x3C;             //Enable pull down resistor on PORTA for rows
  GPIO_PORTC_DEN_R |= 0xF0;
  GPIO_PORTA_DEN_R = 0x3C;
	GPIO_PORTA_AMSEL_R = 0x00;
	GPIO_PORTA_AFSEL_R = 0x00;
	GPIO_PORTA_PCTL_R = 0x00000000;
}


void SystemInit (void) 
{
	// Initialization of Port F
	
		SYSCTL_RCGCGPIO_R |= 0x28; 
		while ((SYSCTL_PRGPIO_R & 0x28)==0); 
		GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; 
		GPIO_PORTF_CR_R |= 0x1F; 
		GPIO_PORTF_AMSEL_R &= ~0x1F; 
		GPIO_PORTF_PCTL_R &= ~0x000FFFFF; 
		GPIO_PORTF_AFSEL_R &= ~0x1F; 
		GPIO_PORTF_DIR_R |= 0x0E; 
		GPIO_PORTF_DEN_R |= 0x1F; 
		GPIO_PORTF_PUR_R |= 0x11;
		GPIO_PORTF_DATA_R |= 0x11;
	
	// Initialization of Port D
	  
	  GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY; 
		GPIO_PORTD_CR_R |= 0x0C; 
		GPIO_PORTD_AMSEL_R &= ~0x0C; 
		GPIO_PORTD_PCTL_R &= ~0x0000FF00; 
		GPIO_PORTD_AFSEL_R &= ~0x0C; 
		GPIO_PORTD_DIR_R |= 0x08; 
		GPIO_PORTD_DEN_R |= 0x0C; 
}



void OUT( unsigned char hundreds , unsigned char tens , unsigned char units) // This is an old function for the 3-digit seven segments which is replaced now by the LCD(BONUS)
{
while(1)
{
    GPIO_PORTE_DATA_R &= 0xF7; //1111 0111
    GPIO_PORTE_DATA_R |= 0x06; //0000 0110
    switch (hundreds)
    {
        case 0 : GPIO_PORTB_DATA_R = 0x7E;
        break;
        case 1 : GPIO_PORTB_DATA_R = 0x30;
        break;
        case 2 : GPIO_PORTB_DATA_R = 0x6D; 
        break;
        case 3 : GPIO_PORTB_DATA_R = 0x79;
        break;
        case 4 : GPIO_PORTB_DATA_R = 0x33;
        break;
        case 5 : GPIO_PORTB_DATA_R = 0x5B;
        break;
        case 6 : GPIO_PORTB_DATA_R = 0x5F;
        break;
        case 7 : GPIO_PORTB_DATA_R = 0x70;
        break;
        case 8 : GPIO_PORTB_DATA_R = 0x7F;
        break;
        case 9 : GPIO_PORTB_DATA_R = 0x73;
        break;
    }
    genericDelay(1);
    GPIO_PORTE_DATA_R &= 0xFB; //1111 1011
    GPIO_PORTE_DATA_R |= 0x0A; //0000 1010
    switch (tens)
    {
        case 0 : GPIO_PORTB_DATA_R = 0x7E;
        break;
        case 1 : GPIO_PORTB_DATA_R = 0x30;
        break;
        case 2 : GPIO_PORTB_DATA_R = 0x6D;
        break;
        case 3 : GPIO_PORTB_DATA_R = 0x79;
        break;
        case 4 : GPIO_PORTB_DATA_R = 0x33;
        break;
        case 5 : GPIO_PORTB_DATA_R = 0x5B;
        break;
        case 6 : GPIO_PORTB_DATA_R = 0x5F;
        break;
        case 7 : GPIO_PORTB_DATA_R = 0x70;
        break;
        case 8 : GPIO_PORTB_DATA_R = 0x7F;
        break;
        case 9 : GPIO_PORTB_DATA_R = 0x73;
        break;
    }
    genericDelay(1);
    GPIO_PORTE_DATA_R &= 0xFD; //1111 1101
    GPIO_PORTE_DATA_R |= 0x0C; //0000 1100
    switch (units)
    {
        case 0 : GPIO_PORTB_DATA_R = 0x7E;
        break;
        case 1 : GPIO_PORTB_DATA_R = 0x30;
        break;
        case 2 : GPIO_PORTB_DATA_R = 0x6D;
        break;
        case 3 : GPIO_PORTB_DATA_R = 0x79;
        break;
        case 4 : GPIO_PORTB_DATA_R = 0x33;
        break;
        case 5 : GPIO_PORTB_DATA_R = 0x5B;
        break;
        case 6 : GPIO_PORTB_DATA_R = 0x5F;
        break;
        case 7 : GPIO_PORTB_DATA_R = 0x70;
        break;
        case 8 : GPIO_PORTB_DATA_R = 0x7F;
        break;
        case 9 : GPIO_PORTB_DATA_R = 0x73;
        break;
    }
    genericDelay(1);
}
}
