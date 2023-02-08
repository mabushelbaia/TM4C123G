# **TM4C123GH6PM Microcontroller**

🔗 | [Datasheet](asset/datasheet.pdf)  
🔗 | [Experiments](Experiments.md)  

# Useful Definetions

## Delay

```c
#define DELAY 900000
void delay( volatile unsigned long ulLoop ){
	for (ulLoop = 0; ulLoop < DELAY; ulLoop++) {
		for (ulLoop = 0; ulLoop < DELAY; ulLoop++) {
		}
	}
}
```

## Enable **GPIOF**

```c
SYSCTL->RCGCGPIO |= 0x20;   // enable clock to GPIOF
GPIOF->LOCK = 0x4C4F434B;   // unlockGPIOCR register
GPIOF->CR = 0x01;           // ENABLE GPIOPUR COMMIT
```

## Enable all pins

```c
#define DEN_ALL 0x1F
#define DIR_ALL 0x0E
GPIOF->DIR = DIR_ALL;
GPIOF->DEN = DEN_ALL;
```

## Switches

```c
#define SW1 0x10
#define SW2 0x01
#define SWITCHES = SW1 + SW2
GPIOF->PUR = SWITCHES;
```

## LED **Colors**

```c
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define YELLOW RED + GREEN
#define MAGENTA BLUE + RED
#define CYAN GREEN + BLUE
#define WHITE RED + GREEN + BLUE
```

# Switch Interrupts

## Setup

```c
GPIOF->IS  &= ~SWITCHES;  // make bit 4, 0 edge sensitive
GPIOF->IBE &= ~SWITCHES;  // trigger is controlled by IEV
GPIOF->IEV &= ~SWITCHES;  // falling edge trigger
GPIOF->ICR |=  SWITCHES;  // clear any prior interrupt
GPIOF->IM  |=  SWITCHES;  // unmask interrupt
NVIC->IP[30] = 3 << 5; 	  // set interrupt priority to 3
NVIC->ISER[0] |= (1<<30); // enable IRQ30 (D30 of ISER[0])
```

## Handler

```c
void GPIOF_Handler(void) {
    if(GPIOF->MIS & SW1) { // SW1 is pressed
		// do something
        GPIOF->ICR |= 0x10;
    }
    else if (GPIOF->MIS & SW2) { // SW2 is pressed
		// do something
        GPIOF->ICR |= 0x01;
    }
}
```

# Timer and Timer Interrupts

## Setup

TM4C123G microcontroller has two timer blocks, TimerA and TimerB, TimerA has six 16/32 bits GPTM, and TimerB has six 32/64 bits GPTM.
RCGCTIMER Register is used to enable the clock to the timer blocks. The timer blocks are disabled by default. The timer blocks are enabled by writing a 1 to the appropriate bit in the RCGCTIMER register.

```c
SYSCTL->RCGCTIMER |= 0x3F;  // enable clock to all timers
TIMER1 -> CTL = 0;          // disable timer ouput
```

```c
TIMER1-> CFG = 0x4; // select 16-bit configuration option
TIMER1-> CFG = 0x0; // select 32-bit configuration option
```

```C
TIMER1->TAPR = 250-1;       // TimerA prescaler value

```
The prescaler values is used to scaled down the frequency of the timer module, originaly the timer module has a frequency of 16MHz, the prescaler can scale it down by 1 - 255, so the timer module will have a frequency of 16MHz/250 = 64KHz.


```c
TIMER1->TAMR = 0x02;        // Periodic count down mode
TIMER1->TAILR = 64000-1;    // Count down value
```
The counter counts down from the value in the TAILR register to 0 at each clock cycle, and then it reloads the value in the TAILR register and continues to count down. When the counter reaches 0,  it generates a time-out interrupt and reloads the value in the TAILR register. Since we reduced the clock to 64KHz, the counter will count down from 64000 to 0 in 1 second.



```c
TIMER1->ICR = 0x1;          // timeout flag bit clears
TIMER1->IMR |=(1<<0);       // enables time-out intrpt mask
TIMER1->CTL |= 0x01;        // enable timer
```
```c
NVIC->ISER[0] |= (1<<21);   // enable IRQ21
```
| TimerA | IRQ  | TimerB | IRQ  |
| :-------: | :--: | :-------: | :--: |
| Timer0A | 19  | Timer0B | 20  |
| Timer1A | 21  | Timer1B | 22  |
| Timer2A | 23  | Timer2B | 24  |
| Timer3A | 35  | Timer3B | 36  |
| Timer4A | 37  | Timer4B | 38  |
| Timer5A | 53  | Timer5B | 54  |

## Handler
```c
TIMER1A_Handler() {
    if(TIMER1->MIS & 0x1) {
        // do something
        TIMER1->ICR = 0x1;  // timeout flag bit clears
    }
}
```
**NOTE**:   
In this example, we used TIMER1 but we can use any other timer, for example TIMER0, TIMER2, TIMER3, TIMER4, TIMER5
You can see an example of using multiple timers [here](Experiment%208/Classwork3.c).
