#include "TM4C123GH6PM.h"
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define SW2 0x01
#define SW1 0x10
#define DELAY 900000
int flag = 1;
void delay( volatile unsigned long i ){for (i = 0; i < DELAY; i++){for (i = 0; i < DELAY; i++){}}}
int main(void) {
    SYSCTL->RCGCGPIO |= 0x20; /* enable clock to GPIOF */
    GPIOF->LOCK = 0x4C4F434B; // unlockGPIOCR register
    GPIOF->CR = 0x01;         // Enable GPIOPUR register enable to commit
    GPIOF->DIR &= ~(1<<4)|~(1<<0); /* Set PF4 and PF0 as a digital input pins */
    GPIOF->DIR = 0x0E; /* Set PF3 as digital output to control green LED */
    GPIOF->DEN = 0x1F; /* make PORTF4-0 digital pins */
    GPIOF->PUR |= (1<<4)|(1<<0); /* enable pull up for PORTF4, 0 */
    GPIOF->IS &= ~(1<<4)|~(1<<0); /* make bit 4, 0 edge sensitive */
    GPIOF->IBE &=~(1<<4)|~(1<<0); /* trigger is controlled by IEV */
    GPIOF->IEV &= ~(1<<4)|~(1<<0); /* falling edge trigger */
    GPIOF->ICR |= (1<<4)|(1<<0); /* clear any prior interrupt */
    GPIOF->IM |= (1<<4)|(1<<0); /* unmask interrupt */
    NVIC->IP[30] = 3 << 5; /* set interrupt priority to 3 */
    NVIC->ISER[0] |= (1<<30); /* enable IRQ30 (D30 of ISER[0]) */
    while(1) {
        if (flag) {
            GPIOF->DATA = RED;
        }
    }
}
void GPIOF_Handler(void) {
    if(GPIOF->MIS & 0x10) {
        GPIOF->DATA = BLUE;
        GPIOF->ICR |= 0x10;
        flag = 0;
    }
    else if (GPIOF->MIS & 0x01) {
        GPIOF->DATA = GREEN;
        GPIOF->ICR |= 0x01;
        flag = 0;
    }
}