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
#define DEN_ALL 0x1F
#define DIR_ALL 0x0E
#define True 1
#define False 0
#define DELAY 900000

int main(void)
{
    SYSCTL->RCGCGPIO |= 0x20;
    GPIOF->LOCK = 0x4C4F434B;
    GPIOF->CR = 0x01;
    GPIOF->DIR = DIR_ALL;
    GPIOF->DEN = DEN_ALL;
    SYSCTL->RCGCTIMER |= 0x3F; // enable clock to all timers
    volatile unsigned long arr[] = {TIMER0, TIMER1, TIMER2};
    int delays[] = {1, 5, 10};
    int for (int i = 0; i < 6; i++)
    {
        arr[i]->CTL = 0;     // disable timer ouput
        arr[i]->CFG = 0x00;  // 32-bit mode
        arr[i]->TAMR = 0x02; // periodic mode
        arr[i]->TAILR = 16 * 1000000 * delays[i] - 1;
        arr[i]->ICR = 0x1;                    // timeout flag bit clears
        arr[i]->IMR |= (1 << 0);              // enables time-out intrpt mask
        arr[i]->CTL |= 0x01;                  // enable timer
        NVIC->ISER[0] |= (1 << (19 + 2 * i)); // enable IRQ21
    }
    while (True)
        ;
}

TIMER0A_Handler()
{
    if (TIMER0->MIS & 0x1)
    {
        GPIOF->DATA ^= GREEN;
        TIMER0->ICR = 0x1; // timeout flag bit clears
    }
}
TIMER1A_Handler()
{
    if (TIMER1->MIS & 0x1)
    {
        GPIOF->DATA ^= RED;
        TIMER1->ICR = 0x1; // timeout flag bit clears
    }
}
TIMER2A_Handler()
{
    if (TIMER2->MIS & 0x1)
    {
        GPIOF->DATA ^= BLUE;
        TIMER2->ICR = 0x1; // timeout flag bit clears
    }
}
