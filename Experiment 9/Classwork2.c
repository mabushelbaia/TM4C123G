#include "TM4C123.h" // Device header
#define LCD GPIOB    // LCD port with Tiva C
#define RS 0x01      // RS -> PB0 (0x01)
#define RW 0x02      // RW -> PB1 (0x02)
#define EN 0x04      // EN -> PB2 (0x04)
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
int cursor = 0x80;
char *str1 = "Mohammad";
// Functions Declaration
void delayUs(int); // Delay in Micro Seconds
void delayMs(int); // Delay in Milli Seconds

void LCD4bits_Init(void);                          // Initialization of LCD Dispaly
void LCD_Write4bits(unsigned char, unsigned char); // Write data as (4 bits) on LCD
void LCD_WriteString(char *);                      // Write a string on LCD
void LCD4bits_Cmd(unsigned char);                  // Write command
void LCD4bits_Data(unsigned char);                 // Write a character
int main(void)
{
    char *str1 = "Mohammad"; // Write any string you want to display on the first row of LCD
    LCD4bits_Init();         // Initialization of LCD
    LCD4bits_Cmd(0x01);      // Clear the display
    LCD4bits_Cmd(0x80);      // Force the cursor to beginning of 1st line
    delayMs(500);            // delay 500ms for LCD (MCU is faster than LCD)
    LCD_WriteString(str1);   // Write the string on LCD
    delayMs(500);            // Delay 500 ms to let the LCD diplays the data
    while (True)
        ; // Infinite loop
}
void LCD4bits_Init(void)
{
    SYSCTL->RCGCGPIO |= 0x20; // enable clock to GPIOF
    GPIOF->LOCK = 0x4C4F434B; // unlockGPIOCR register
    GPIOF->CR = 0x01;         // ENABLE GPIOPUR COMMIT
    SYSCTL->RCGCGPIO |= 0x02; // enable clock for PORTB
    GPIOF->PUR = SWITCHES;
    GPIOF->DIR = DIR_ALL;
    GPIOF->DEN = DEN_ALL;
    GPIOF->IS &= ~SWITCHES;     // make bit 4, 0 edge sensitive
    GPIOF->IBE &= ~SWITCHES;    // trigger is controlled by IEV
    GPIOF->IEV &= ~SWITCHES;    // falling edge trigger
    GPIOF->ICR |= SWITCHES;     // clear any prior interrupt
    GPIOF->IM |= SWITCHES;      // unmask interrupt
    NVIC->IP[30] = 3 << 5;      // set interrupt priority to 3
    NVIC->ISER[0] |= (1 << 30); // enable IRQ30 (D30 of ISER[0])
    delayMs(10);                // delay 10 ms for enable the clock of PORTB
    LCD->DIR = 0xFF;            // let PORTB as output pins
    LCD->DEN = 0xFF;            // enable PORTB digital IO pins
    LCD4bits_Cmd(0x28);         // 2 lines and 5x7 character (4-bit data, D4 to D7)
    LCD4bits_Cmd(0x06);         // Automatic Increment cursor (shift cursor to right)
    LCD4bits_Cmd(0x01);         // Clear display screen
    LCD4bits_Cmd(0x0F);         // Display on, cursor blinking
}
void LCD_Write4bits(unsigned char data, unsigned char control)
{
    data &= 0xF0;                    // clear lower nibble for control
    control &= 0x0F;                 // clear upper nibble for data
    LCD->DATA = data | control;      // Include RS value (command or data ) with data
    LCD->DATA = data | control | EN; // pulse EN
    delayUs(0);                      // delay for pulsing EN
    LCD->DATA = data | control;      // Turn off the pulse EN
    LCD->DATA = 0;                   // Clear the Data
}
void LCD_WriteString(char *str)
{
    volatile int i = 0;        // volatile is important
    while (*(str + i) != '\0') // until the end of the string
    {
        LCD4bits_Data(*(str + i)); // Write each character of string
        i++;                       // increment for next character
    }
}
void LCD4bits_Cmd(unsigned char command)
{
    LCD_Write4bits(command & 0xF0, 0); // upper nibble first
    LCD_Write4bits(command << 4, 0);   // then lower nibble
    if (command < 4)
        delayMs(2); // commands 1 and 2 need up to 1.64ms
    else
        delayUs(40); // all others 40 us
}
void LCD4bits_Data(unsigned char data)
{
    LCD_Write4bits(data & 0xF0, RS); // upper nibble first
    LCD_Write4bits(data << 4, RS);   // then lower nibble
    delayUs(40);                     // delay for LCD (MCU is faster than LCD)
}
void delayMs(int n)
{
    volatile int i, j; // volatile is important for variables incremented in code
    for (i = 0; i < n; i++)
        for (j = 0; j < 3180; j++) // delay for 1 msec
        {
        }
}
void delayUs(int n)
{
    volatile int i, j; // volatile is important for variables incremented in code
    for (i = 0; i < n; i++)
        for (j = 0; j < 3; j++) // delay for 1 micro second
        {
        }
}

void GPIOF_Handler(void)
{
    if (GPIOF->MIS & SW1)
    { // SW1 is pressed
        unsigned char displacment = 0;
        while (GPOIF->MIS & SW2)
        {
            LCD4bits_Cmd(0x01); // Clear the display
            cursor = cursor + (((displacment++) + 9) % 16);
            LCD4bits_Cmd(cursor);  // Force the cursor to beginning of 1st line
            delayMs(500);          // delay 500ms for LCD (MCU is faster than LCD)
            LCD_WriteString(str1); // Write the string on LCD
            delayMs(500);          // Delay 500 ms to let the LCD diplays the data
        }
        GPIOF->ICR |= 0x10;
    }
    else if (GPIOF->MIS & SW2)
    { // SW2 is
        unsigned char displacment = 0;
        while (GPOIF->MIS & SW1)
        {
            LCD4bits_Cmd(0x01); // Clear the display
            cursor = cursor - (((displacment++) + 9) % 16);
            if (cursor < 0x80)
                cursor = 0x9F;
            LCD4bits_Cmd(cursor);  // Force the cursor to beginning of 1st line
            delayMs(500);          // delay 500ms for LCD (MCU is faster than LCD)
            LCD_WriteString(str1); // Write the string on LCD
            delayMs(500);          // Delay 500 ms to let the LCD diplays the data
        }
        GPIOF->ICR |= 0x01;
    }
}