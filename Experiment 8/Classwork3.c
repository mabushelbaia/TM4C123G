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

int main(void)
{
    SYSCTL->RCGCGPIO |= 0x20;
    GPIOF->LOCK = 0x4C4F434B;
    GPIOF->CR = 0x01;
    GPIOF->DIR = DIR_ALL;
    GPIOF->DEN = DEN_ALL;
    SYSCTL->RCGCTIMER |= (1<<0) | (1<<1) | (1<<2); // enable clock to all timers

    
    TIMER0->CTL = 0;     // disable timer ouput
    TIMER0->CFG = 0x00;  // 32-bit mode
    TIMER0->TAMR = 0x02; // periodic mode
    TIMER0->TAILR = 16 * 1000000 * 1 - 1;
    TIMER0->ICR = 0x1;                    // timeout flag bit clears
    TIMER0->IMR |= (1 << 0);              // enables time-out intrpt mask
    TIMER0->CTL |= 0x01;                  // enable timer
    NVIC->ISER[0] |= (1 << (19)); // enable IRQ21

    
    TIMER1->CTL = 0;     // disable timer ouput
    TIMER1->CFG = 0x00;  // 32-bit mode
    TIMER1->TAMR = 0x02; // periodic mode
    TIMER1->TAILR = 16 * 1000000 * 5 - 1;
    TIMER1->ICR = 0x1;                    // timeout flag bit clears
    TIMER1->IMR |= (1 << 0);              // enables time-out intrpt mask
    TIMER1->CTL |= 0x01;                  // enable timer
    NVIC->ISER[0] |= (1 << (21)); // enable IRQ21

    
    TIMER2->CTL = 0;     // disable timer ouput
    TIMER2->CFG = 0x00;  // 32-bit mode
    TIMER2->TAMR = 0x02; // periodic mode
    TIMER2->TAILR = 16 * 1000000 * 10 - 1;
    TIMER2->ICR = 0x1;                    // timeout flag bit clears
    TIMER2->IMR |= (1 << 0);              // enables time-out intrpt mask
    TIMER2->CTL |= 0x01;                  // enable timer
    NVIC->ISER[0] |= (1 << (23)); // enable IRQ21

    while (True);
}

void TIMER0A_Handler()
{
    if (TIMER0->MIS & 0x1)
    {
        GPIOF->DATA = GREEN;
        TIMER0->ICR = 0x1; // timeout flag bit clears
    }
}
void TIMER1A_Handler()
{
    if (TIMER1->MIS & 0x1)
    {
        GPIOF->DATA = RED;
        TIMER1->ICR = 0x1; // timeout flag bit clears
    }
}
void TIMER2A_Handler()
{
    if (TIMER2->MIS & 0x1)
    {
        GPIOF->DATA = BLUE;
        TIMER2->ICR = 0x1; // timeout flag bit clears
    }
}
