
// Andrzej Ganczarek
// Runs on TM4C1234GH6PM
// Program for set connection via UART

/*
PF1, PF2, PF3 as output, connected to LED
When press on computer keyboard 
	r - turn on LED red
	g- turn on LED green
	b - turn on LED blue


All devices are on board TIVA C EK-TM4C1234GXL
*/


#include <stdint.h>
#include "C:\Keil_v5\tm4c123gh6pm.h"
// Andrzej Ganczarek
// Runs on TM4C1234GH6PM



void initUART(void);
void initGPIOF(void);
void printChar(char c);
char readChar(void);
void printString(char *string);


int main(void) {
char c;
initGPIOF();
initUART();


while(1) {

	printString("Enter \"r\", \"g\", or \"b\":\n\r");
c = readChar();
printChar(c);
printString("\n\r");
switch(c) {
case 'r':
GPIO_PORTF_DATA_R = 0x02;
break;
case 'b':
GPIO_PORTF_DATA_R = 0x04;
break;
case 'g':
GPIO_PORTF_DATA_R = 0x08;
break;
default:
GPIO_PORTF_DATA_R &= ~0x0E;
break;
}



/*
//void GPIOPortF_Handler(void)
//8. Poll the GPTMRIS register or wait for the interrupt to be generated (if enabled). In both cases,
//the status flags are cleared by writing a 1 to the appropriate bit of the GPTM Interrupt Clear
//Register (GPTMICR).
if((TIMER0_RIS_R & 0x0001) == 0x01) {
TIMER0_ICR_R |= 0x01;
GPIO_PORTF_DATA_R ^= 0x08;
}

*/

}

//return 0;
}




void initGPIOF(void) {
SYSCTL_RCGCGPIO_R |=0x20; //active clock on port F
GPIO_PORTF_DIR_R |= 0x0E; //set PF1-3 as output
GPIO_PORTF_AFSEL_R &= ~0x0E; //disable alternative function on PF1-33
GPIO_PORTF_PCTL_R &= ~0x0E;  //configure PF1-3 as GPIO
GPIO_PORTF_DEN_R |= 0x0E; //enable PF1-3 as digital I/Os
GPIO_PORTF_AMSEL_R &= ~0x0E; //disable analogue function on PF1-3
}



void initUART(void) {
//1. Enable the UART module using the RCGCUART register (see page 344).
SYSCTL_RCGCUART_R |= 0x01;

//2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
//To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
SYSCTL_RCGCGPIO_R |= 0x01;

//3. Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
//configure, see Table 23-4 on page 1344.
GPIO_PORTA_AFSEL_R |= 0x03;

//4. Configure the GPIO current level and/or slew rate as specified for the mode selected (see
//page 673 and page 681).


//5. Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
//pins (see page 688 and Table 23-5 on page 1351).
GPIO_PORTA_PCTL_R |= 0x11;

GPIO_PORTA_DEN_R |= 0x03;

//1. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
UART0_CTL_R &= ~0x01;

//2. Write the integer portion of the BRD to the UARTIBRD register.
UART0_IBRD_R = 104;

//3. Write the fractional portion of the BRD to the UARTFBRD register.
UART0_FBRD_R = 11;

//4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of
//0x0000.0060).
UART0_LCRH_R = (0x3<<5);

//5. Configure the UART clock source by writing to the UARTCC register.
UART0_CC_R = 0x0;

//6. Optionally, configure the µDMA channel (see “Micro Direct Memory Access (µDMA)” on page 585)
//and enable the DMA option(s) in the UARTDMACTL register.


//7. Enable the UART by setting the UARTEN bit in the UARTCTL register.
UART0_CTL_R = (1<<0)|(1<<8)|(1<<9);

}


void printChar(char c) {
while((UART0_FR_R & 0x20) != 0x00);
UART0_DR_R = c;

}

char readChar(void) {
char c;
while((UART0_FR_R & 0x10) != 0x00);
c = UART0_DR_R;
return c;
}

void printString(char *string){
while(*string){
printChar(*(string++));
}
}

