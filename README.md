# TM4C123GH6PM Microcontroller

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

## Enable GPIOF

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
GPIOF->PUR = SW1 + SW2;
```

## LED Colors

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
#define SWITCHES = SW1 + SW2
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

# Timer Interrupts

## Setup

```c

```
## Handler

```c

```