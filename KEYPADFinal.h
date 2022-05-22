#ifndef KEYPADFinal_H_
#define KEYPADFinal_H_

void Port_AC_Init();
void Keypad_Init(void);
unsigned char Keypad_Getkey(void);
void SysTick_wait(unsigned long delay);
void genericDelay(unsigned long time);

#endif /* KPD_H_ */