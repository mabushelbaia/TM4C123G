// Program 1
#include "TM4C123GH6PM.h"
#define OFF 0x00
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define YELLOW RED + GREEN
#define MAGENTA BLUE + RED
#define CYAN GREEN + BLUE
#define WHITE RED + GREEN + BLUE
#define SW2 0x01
#define SW1 0x10
#define SWITCHES (SW1 | SW2)
#define ENABLE_ALL 0x1F
#define DIR_ALL 0x0E
#define True 1
#define False 0
#define DELAY 900000
unsigned int arr[] = {BLUE, RED, GREEN};
unsigned int arr_2[] = {YELLOW, MAGENTA, CYAN};
int stop_flag = 0;
int counter = 0;
void delay( volatile unsigned long ulLoop ){
	
		// Delay for a bit.
		for (ulLoop = 0; ulLoop < DELAY; ulLoop++)
			{
			for (ulLoop = 0; ulLoop < DELAY; ulLoop++)
			{}
			}
}
int main(void) {
    SYSCTL->RCGCGPIO |= 0x20; /* enable clock to GPIOF */
    GPIOF->LOCK = 0x4C4F434B; // unlockGPIOCR register
    GPIOF->CR = 0x01;         // Enable GPIOPUR register enable to commit
    GPIOF->DIR &= ~(1<<4)|~(1<<0); /* Set PF4 and PF0 as a digital input pins */
    GPIOF->DIR = 0x0E; /* Set PF3 as digital output to control green LED */
    GPIOF->DEN = 0x1F; /* make PORTF4-0 digital pins */
    GPIOF->PUR |= (1<<4)|(1<<0); /* enable pull up for PORTF4, 0 */
    /* configure PORTF4, 0 for falling edge trigger interrupt */
    GPIOF->IS &= ~(1<<4)|~(1<<0); /* make bit 4, 0 edge sensitive */
    GPIOF->IBE &=~(1<<4)|~(1<<0); /* trigger is controlled by IEV */
    GPIOF->IEV &= ~(1<<4)|~(1<<0); /* falling edge trigger */
    GPIOF->ICR |= (1<<4)|(1<<0); /* clear any prior interrupt */
    GPIOF->IM |= (1<<4)|(1<<0); /* unmask interrupt */
    
    /* enable interrupt in NVIC and set priority to 3 */
    NVIC->IP[30] = 3 << 5; /* set interrupt priority to 3 */
    NVIC->ISER[0] |= (1<<30); /* enable IRQ30 (D30 of ISER[0]) */
    int i = 0;
    while (1) {
        if (!stop_flag) {
            GPIOF->DATA = arr[i];
            delay(0);
            i = (i + 1)%3;
        }

    }

}
void GPIOF_Handler(void) {
    if(GPIOF->MIS & 0x10) {
        GPIOF->DATA = arr_2[counter];
        counter = (counter + 1)%3;
        stop_flag = 1;
        GPIOF->ICR |= 0x10;
    }
    else if (GPIOF->MIS & 0x01) {
        GPIOF->DATA = WHITE;
        stop_flag = 1;
        GPIOF->ICR |= 0x01;
    }
}
