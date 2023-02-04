#include "TM4C123GH6PM.h"
#define GREEN 0x08
#define SW1 0x01
#define SW2 0x10
#define True 1
int main(void)
{
	unsigned int state;
	SYSCTL->RCGCGPIO |= 0x20;	 /* enable clock to GPIOF */
	GPIOF->LOCK = 0x4C4F434B;	 // unlockGPIOCR register
	GPIOF->CR = 0x01;			 // Enable GPIOPUR register enable to commit
	GPIOF->PUR |= 0x10;			 // Enable Pull Up resistor SW2
	GPIOF->DIR |= GREEN;		 // set GREEN as an output and SW2 as an input pin
	GPIOF->DEN |= (GREEN + SW2); // Enable SW2 and GREEN as a digital GPIO pins
	while (True)
	{
		state = GPIOF->DATA & 0x10;
		if (!state)
		{
			GPIOF->DATA ^= GREEN;
		}
	}
}