#ifndef GPIOFinal_H_
#define GPIOFinal_H_


/* PIN Defines */
#define PIN0   0
#define PIN1   1
#define PIN2   2
#define PIN3   3
#define PIN4   4
#define PIN5   5
#define PIN6   6
#define PIN7   7

/* PIN Value Options */
#define HIGH   1
#define LOW    0

/* IO Ports Initialization Functions Prototypes*/
void SystemInit (void);
void PortE_Init (void);
void PortB_Init (void);
void Port_AC_Init(void);
void OUT(unsigned char hundreds, unsigned char tens, unsigned char units); // If used 3-digit seven segments instead of LCD
void genericDelay(unsigned long time);

#endif