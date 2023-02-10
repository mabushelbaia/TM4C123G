#include "TM4C123GH6PM.h"
#define OFF 0x00
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define SW2 0x01
#define SW1 0x10
#define True 1
#define False 0
#define DELAY 900000
int sequence_flag = 0;
unsigned int arr[] = {RED, BLUE, GREEN};
int i = 0;
void delay(volatile unsigned long i)
{
    for (i = 0; i < DELAY; i++)
    {
        for (i = 0; i < DELAY; i++)
        {
        }
    }
}
int main(void)
{
    SYSCTL->RCGCGPIO |= 0x20;            /* enable clock to GPIOF */
    GPIOF->LOCK = 0x4C4F434B;            // unlockGPIOCR register
    GPIOF->CR = 0x01;                    // Enable GPIOPUR register enable to commit
    GPIOF->DIR &= ~(1 << 4) | ~(1 << 0); /* Set PF4 and PF0 as a digital input pins */
    GPIOF->DIR = 0x0E;                   /* Set PF3 as digital output to control green LED */
    GPIOF->DEN = 0x1F;                   /* make PORTF4-0 digital pins */
    GPIOF->PUR |= (1 << 4) | (1 << 0);   /* enable pull up for PORTF4, 0 */
    /* configure PORTF4, 0 for falling edge trigger interrupt */
    GPIOF->IS &= ~(1 << 4) | ~(1 << 0);  /* make bit 4, 0 edge sensitive */
    GPIOF->IBE &= ~(1 << 4) | ~(1 << 0); /* trigger is controlled by IEV */
    GPIOF->IEV &= ~(1 << 4) | ~(1 << 0); /* falling edge trigger */
    GPIOF->ICR |= (1 << 4) | (1 << 0);   /* clear any prior interrupt */
    GPIOF->IM |= (1 << 4) | (1 << 0);    /* unmask interrupt */
    /* enable interrupt in NVIC and set priority to 3 */
    NVIC->IP[30] = 3 << 5;      /* set interrupt priority to 3 */
    NVIC->ISER[0] |= (1 << 30); /* enable IRQ30 (D30 of ISER[0]) */
    while (True)
    {
        if (sequence_flag == 1)
        {
            delay(0);
            GPIOF->DATA = arr[i];
            i = (i + 1) % 3;
        }
        if (sequence_flag == 2)
        {
            delay(0);
            GPIOF->DATA = arr[2 - i];
            i = (i + 1) % 3;
        }
    }
}

void GPIOF_Handler(void)
{
    if (GPIOF->MIS & 0x10)
    {
        sequence_flag = 1;
        i = 0;
        GPIOF->ICR |= 0x10;
    }
    else if (GPIOF->MIS & 0x01)
    {
        sequence_flag = 2;
        i = 0;
        GPIOF->ICR |= 0x01;
    }
}