#include "tm4c123gh6pm.h"
#include "LCDFinal.h"
#include "STDFinal.h"
#include "GPIOFinal.h"
#include "KEYPADFinal.h"
#include "stdio.h"

// Global variables that are used in more than one function 
unsigned char SW1_clicked;
unsigned char SW2_clicked;
unsigned char SW3_clicked;
char Pause_min1, Pause_min2, Pause_sec1, Pause_sec2, min1, min2, sec1, sec2;
char Input;
char weight;
char In1, In2, In3, In4;
char Arr[5];


//Prototype for functions 
void Count_Down(char min1, char min2, char sec1, char sec2);
int main();

char get_Command()   // get command from keypad A or B or C or D 
{
	char character; 
	character = Keypad_Getkey();
	return (char) (character);              // function returns character received by keypad
}

// Switches functions 
unsigned char SW1_Pressed(void)  
{
	return GPIO_PORTF_DATA_R & 0x10;                    // SW1 is PF4
}

unsigned char SW2_Pressed(void)
{
		return GPIO_PORTF_DATA_R & 0x01;                 // SW2 is PF0
}

unsigned char SW3_Pressed(void)
{
		return GPIO_PORTD_DATA_R & 0x04;                 // SW3 is PD2 and buzzer is also connected to PD3  
}

// Start Cooking function shows if i am ready to cook and calls cunt down function 
int Start_Cooking(char min1, char min2, char sec1, char sec2)        
{
	while(1)
	{
		SW1_clicked = SW1_Pressed();
		SW2_clicked = SW2_Pressed();
		SW3_clicked = SW3_Pressed();
		
		if ((SW2_clicked != 0x01) && (SW3_clicked == 0x04) )     // SW2 is pressed to start cooking --> check SW3 is not pressed --> Door Closed 
		{	
		GPIO_PORTF_DATA_R |= 0x0E;          // Turn on array of LEDS 
		Count_Down(min1, min2,sec1,sec2);   // take time wanted and call count down fumction to start cooking 
  	        }
        }
}

// Finishing cook calls End Cooking function 
void End_Cooking()       
{
			int j;
			for (j=6;j>0;j--)
			{
				GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^(0x0E);          // Flash LEDS 3 times
				GPIO_PORTD_DATA_R = GPIO_PORTD_DATA_R ^(0x08);          // Buzzer 3 times 
				genericDelay(500);                                      
			}
			lcd_cmd(lcd_Clear);              // clear the LCD
			GPIO_PORTF_DATA_R &= 0x11;       // turn off all leds
			main();
}

// if at any time door is opened or if SW1 is pressed Cooking is Paused by Pause Cooking function 
void Pause_Cooking (char Pause_min1,char Pause_min2,char Pause_sec1,char Pause_sec2)      // Pause Cooking if door opened or switch 1 is clicked 
{ 
	while(1)
	{
			SW1_clicked = SW1_Pressed();
		  SW2_clicked = SW2_Pressed();
			SW3_clicked = SW3_Pressed();
		
		  if ((SW2_clicked != 0x01) && (SW3_clicked == 0x04))     // to resume cooking --> SW2 is Pressed & SW3 is not Pressed (Door is closed) 
		{              
			Start_Cooking(Pause_min1, Pause_min2, Pause_sec1, Pause_sec2);      // resume counting down using the paused time 
		}
			else if ((SW1_clicked != 0x10))          // SW1 is Pressed 
		{
			lcd_cmd(lcd_Clear);      // Clear the LCD 
		  End_Cooking();           // go to End Cooking 
		}
		else                                                // If still in Paused Condition 
		{
			GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^(0x0E);   // LEDS Blink 
			genericDelay(500);
		}	
	}	
}

// Count down function that is called if Start Cooking is called 
void Count_Down(char min1, char min2, char sec1, char sec2)             //min1 min2 : sec1 sec2
{
    while(1)
    {
					SW1_clicked = SW1_Pressed();
		      SW2_clicked = SW2_Pressed();
					SW3_clicked = SW3_Pressed();
			
          sec2 = sec2 - 1;
          genericDelay(1000);
          lcd_cmd(lcd_Clear);
        
      if (sec2 == '/')                // '/' because it is the ASCII before '0' --> LCD displays ASCII characters 
      {
					sec2= '9';
					sec1 = sec1-1;    
      }
            
      if (sec1 == '/' )
      {
           sec1= '5';
					 min2 = min2-1;    
      }
            if (min2 == '/')
      {
           min2= '9';
           min1 = min1-1;    
      }
            if (min1 == '/')
      {
           min1= '5';    
      }
            
          lcd_write(min1);            // Display time after decrementing it 
          lcd_write(min2);
					lcd_write(':');
          lcd_cmd(lcd_Cursor_Right);
          lcd_write(sec1);
          lcd_write(sec2);
			   
					if( sec1 == '0' & min1 == '0'  & sec2 == '0' & min2 == '0')    // if time ends go to End Cooking 
					{
								End_Cooking();
                break;
					}
					else if((SW1_clicked != 0x10) || (SW3_clicked != 0x04))        // if time Paused or Door got opened go to Pause Cooking 
					{
							Pause_min1 = min1;
							Pause_min2 = min2;
							Pause_sec1 = sec1;
							Pause_sec2 = sec2;
						
							Pause_Cooking(Pause_min1, Pause_min2, Pause_sec1, Pause_sec2); 
					}
				}
}
// Case A	
void Case_A()
{
		lcd_write_string("Popcorn");
		lcd_cmd(first_line2);
		lcd_write_string("01:00");	
		Start_Cooking('0','1','0','0');                                    
}

// converting weight input in case B or C into time 
char weight_time()
{
	char m1,m2,s1,s2,min,sec;
	int rate, time,i;
	
				if (Input == 'B') rate = 30;
				else rate = 12;
				min = 0;	
				time = (weight-48)*rate;	
				while(time >= 60)
				{
						time = time - 60;
						min = min + 1;
				}
					sec = time;
					
					m1 = '0';                    // largest number entered = 9*0.5 = 4.5 
					m2 = min + 48;
					s2 = (sec % 10) + 48;        // takes second digit 
					s1 = (sec / 10) + 48;
					
					Arr[0] = m1;
					Arr[1] = m2;
					Arr[2] = ':';
					Arr[3] = s1;
					Arr[4] = s2;
					lcd_cmd(lcd_Clear);
					lcd_write_string(Arr);
				
	while(1)
		{
			SW2_clicked = SW2_Pressed();
		
			if ((SW2_clicked != 0x01))
				{					
					Start_Cooking(m1,m2,s1,s2);
				}
		}
}

// case B & C 
void Case_BC()
{
		int i;
		char NotValid [] = {'A','B','C','D','*','0','#'};
		char wrong_found = 0;
		
		if(Input == 'B')
		{
			lcd_write_string("Beef weight?");
			weight = get_Command();
		}
		
		else 
		{
			lcd_write_string("Chicken weight?");
			weight = get_Command();
		}
			
		 // Checking for Error 
		for ( i = 0; i < 7; i++)
		{
			if(NotValid[i] == weight)
			{
				wrong_found = 1;
				break;
			}
		}			
	
		while(1)
		{
			if(wrong_found)
			{
					lcd_cmd(lcd_Clear);
					lcd_write_string("Err");
					genericDelay(2000);
					lcd_cmd(lcd_Clear);
					Case_BC();
			}
			
			else if(wrong_found == 0)
			{
				lcd_cmd(lcd_Clear);
				lcd_write(weight);
				genericDelay(2000);
				weight_time();
			}
		}			
}
	
// case D 	
void Case_D()
{	
	  lcd_write_string("Cooking Time?");
		genericDelay(1000);
		lcd_cmd(lcd_Clear);
	  Arr[0] = '0';
		Arr[1] = '0';
   	Arr[2] = ':';
		Arr[3] = '0';
		Arr[4] = '0';
    lcd_cmd(lcd_Home);
		lcd_write_string(Arr);
		In1 = get_Command();
		Arr[4] = In1;
		lcd_cmd(lcd_Home);
		lcd_write_string(Arr);
	 
		Arr[3] = Arr[4];
		In2 = get_Command();
		Arr[4] = In2;		
		lcd_cmd(lcd_Home);
		lcd_write_string(Arr);
	   
	  Arr[1] = Arr[3];
		Arr[3] = Arr[4];
		In3 = get_Command();
		Arr[4] = In3;
		lcd_cmd(lcd_Home);
		lcd_write_string(Arr);
	
	  Arr[0] = Arr[1];
		Arr[1] = Arr[3];
		Arr[3] = Arr[4];
		In4 = get_Command();
		Arr[4] = In4;
		lcd_cmd(lcd_Home);
		lcd_write_string(Arr);
		
	  min1 = Arr[0];
	  min2 = Arr[1];
	  sec1 = Arr[3];
	  sec2 = Arr[4];
	  
	  // Check if time entered is valid --> between 1 min & 30 mins
	 if((min1>'3') || ((min1 == '0')&& (min2<'1')) || ((min1=='3') && ((min2>='1') || (sec1>='1') || (sec2>='1')))  || (sec1>'5'))
	 {
			lcd_cmd(lcd_Clear);
			lcd_write_string("Enter a value");
			lcd_cmd(first_line2);
			lcd_write_string("bet 1-30 mins");
			genericDelay(1000);
			lcd_cmd(lcd_Clear);
			Case_D();
	 }

		while (1)
		{
			SW1_clicked = SW1_Pressed();
			SW2_clicked = SW2_Pressed();
			SW3_clicked = SW3_Pressed();		

			if ((SW2_clicked != 0x01) && (SW3_clicked == 0x04)) 
			{	
				 Start_Cooking(min1, min2, sec1, sec2);
			}
			
			else if((SW1_clicked != 0x10))
			{
				 lcd_cmd(lcd_Clear);
				 Case_D();
			}
		}
	}	
		
// main function 
int main()
{
		SystemInit ();
		PortE_Init ();
		PortB_Init ();
		Port_AC_Init();
		init_lcd();
	  
	  
		lcd_write_string("Please Enter:");
		lcd_cmd(first_line2);
		lcd_write_string("A B C or D?");
		Input = get_Command();
		lcd_cmd(lcd_Clear);
	
		switch(Input)
		{
			case('A'):
				Case_A();
				break;
		
			case('B'):
				Case_BC();
				break;
			
			case('C'):
				Case_BC();
				break;


			case ('D'):
				Case_D();
				break;
		}
}
