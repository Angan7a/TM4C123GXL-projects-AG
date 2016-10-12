
// Andrzej Ganczarek
// Runs on TM4C1234GH6PM
// Program for initialisation GPTM and toggle LED every 1s

/*
PF3 as output, connected to LED

All devices are on board TIVA C EK-TM4C1234GXL
*/

#include "C:\Keil_v5\tm4c123gh6pm.h" 
#include <stdint.h>

void initGPTM(void);
void initGPIOF(void);

int main(void) {
int i=0;
initGPIOF();
initGPTM();

	
while(1) {
//8. Poll the GPTMRIS register or wait for the interrupt to be generated (if enabled). In both cases,
//the status flags are cleared by writing a 1 to the appropriate bit of the GPTM Interrupt Clear
//Register (GPTMICR).
if((TIMER0_RIS_R & 0x000001) == 0x01) {
TIMER0_ICR_R |= 0x01;
//GPIO_PORTF_DATA_R ^= 0x04;

	
	if (i==0) GPIO_PORTF_DATA_R = 0x02;
	else if (i==2) GPIO_PORTF_DATA_R = 0x04;
	else if (i==4) GPIO_PORTF_DATA_R = 0x08;
	else GPIO_PORTF_DATA_R = 0;
	
	i++;
	if (i>=6) i=0;

	}

}


}



void initGPIOF(void) {
SYSCTL_RCGCGPIO_R |=0x20; //active clock on port F
GPIO_PORTF_DIR_R |= 0x0E; //set PF3 as output
GPIO_PORTF_AFSEL_R &= ~0x0E; //disable alternative function on PF3
GPIO_PORTF_PCTL_R &= ~0x0E;  //configure PF3 as GPIO
GPIO_PORTF_DEN_R |= 0x0E; //enable PF3 as digital I/Os
GPIO_PORTF_AMSEL_R &= ~0x0E; //disable analogue function on PF3
}




void initGPTM(void) {
//To use a GPTM, the appropriate TIMERn bit must be set in the RCGCTIMER or RCGCWTIMER
//register (see page 338 and page 357).
SYSCTL_RCGCTIMER_R |= 0x01;   //set clock

// 1. Ensure the timer is disabled (the TnEN bit in the GPTMCTL register is cleared) before making
//any changes.
TIMER0_CTL_R &= ~0x01;   //disable the timer

//2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000.
TIMER0_CFG_R = 0x00000000;

//3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
//a. Write a value of 0x1 for One-Shot mode.
//b. Write a value of 0x2 for Periodic mode.
TIMER0_TAMR_R |= 0x2; //Periodic mode

//4. Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIR bits in the GPTMTnMR register
//to select whether to capture the value of the free-running timer at time-out, use an external
//trigger to start counting, configure an additional trigger or interrupt, and count up or down.
TIMER0_TAMR_R &= ~0x10;   //counting down

//5. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).
TIMER0_TAILR_R = 0x00F42400;  //start counting value from 16,000,000

//6. If interrupts are required, set the appropriate bits in the GPTM Interrupt Mask Register
//(GPTMIMR).


//7. Set the TnEN bit in the GPTMCTL register to enable the timer and start counting.
TIMER0_CTL_R |= 0x01; //start counting

}

