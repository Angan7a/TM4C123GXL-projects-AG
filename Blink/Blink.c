// Program for blinking LED after press switch
// Runs on TM4C123
// Andrzej Ganczarek

// All devices are on board Tm4C123GXL
// PF0, PF4 - as input, connectet to switchs
// PF1, PF2, PF3 - as output, connected to LED's
//*******************
// If press sw1, LED on PF1 blinking
// If press sw1, LED on PF2 blinking
// If press sw1 and sw2, LED on PF3 blinking
///**********************

#include <stdint.h>
#include "C:\Keil_v5\tm4c123gh6pm.h"

void GPIO_Init(void);
void Delay(void);
void Blink(int);


int main(void){
GPIO_Init();           // initialize GPIO Port F
  while(1){
		if((GPIO_PORTF_DATA_R&0x11) == 0x01) {    //press sw1
			Blink(0x02);   //turn on LED on PF1
		}
		if((GPIO_PORTF_DATA_R&0x11)==0x10 ) {    //press sw2
			Blink(0x04);   //turn on LED on PF2
		}
		if((GPIO_PORTF_DATA_R&0x11)==0x00 ) {    //press sw1 and sw2
			Blink(0x08);   //turn on LED on PF3
		}
	}
	
}

void GPIO_Init(void){   
//1. Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register (see
//page 340). In addition, the SCGCGPIO and DCGCGPIO registers can be programmed in the
//same manner to enable clocking in Sleep and Deep-Sleep modes.
SYSCTL_RCGCGPIO_R |=0x20; //active clock on port F
GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock PortF PF0
GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
//2. Set the direction of the GPIO port pins by programming the GPIODIR register. A write of a 1
//indicates output and a write of a 0 indicates input.
GPIO_PORTF_DIR_R &= ~0x11; //set PF0 and PF4 as input
GPIO_PORTF_DIR_R |= 0x0E; //set PF1, PF2 and PF3 as output
//3. Configure the GPIOAFSEL register to program each bit as a GPIO or alternate pin. If an alternate
//pin is chosen for a bit, then the PMCx field must be programmed in the GPIOPCTL register for
//the specific peripheral required. There are also two registers, GPIOADCCTL and GPIODMACTL,
//which can be used to program a GPIO pin as a ADC or µDMA trigger, respectively.
GPIO_PORTF_AFSEL_R &= ~0x1F; //disable alternative function on PF0 - PF4
GPIO_PORTF_PCTL_R &= ~0x1F;  //configure PF0 - PF4 as GPIO
//4. Set the drive strength for each of the pins through the GPIODR2R, GPIODR4R, and GPIODR8R
//registers.
//GPIO_PORTF_DR2R_R |= 0x1F;  //2mA on output (as default)

//5. Program each pad in the port to have either pull-up, pull-down, or open drain functionality through
//the GPIOPUR, GPIOPDR, GPIOODR register. Slew rate may also be programmed, if needed,
//through the GPIOSLR register.
GPIO_PORTF_PUR_R |= 0x11; //enable weak pull-up on PF0 and PF4
//6. To enable GPIO pins as digital I/Os, set the appropriate DEN bit in the GPIODEN register. To
//enable GPIO pins to their analog function (if available), set the GPIOAMSEL bit in the
//GPIOAMSEL register.
GPIO_PORTF_DEN_R |= 0x1F; //enable PF0 - PF4 as digital I/Os
GPIO_PORTF_AMSEL_R &= ~0x1F ; //disable analogue function on PF0 - PF4
//7. Program the GPIOIS, GPIOIBE, GPIOEV, and GPIOIM registers to configure the type, event,
//and mask of the interrupts for each port.
//Note: To prevent false interrupts, the following steps should be taken when re-configuring
//GPIO edge and interrupt sense registers:
//a. Mask the corresponding port by clearing the IME field in the GPIOIM register.
//b. Configure the IS field in the GPIOIS register and the IBE field in the GPIOIBE
//register.
//c. Clear the GPIORIS register.
//d. Unmask the port by setting the IME field in the GPIOIM register.

//I'm not using interrupt

//8. Optionally, software can lock the configurations of the NMI and JTAG/SWD pins on the GPIO
//port pins, by setting the LOCK bits in the GPIOLOCK register.

//Is done in item 1.


}


void Delay(void){unsigned long volatile time;
time = 2*727240*200/(91*8); 
  while(time){
time--;
  }
}

void Blink(int which_LED){
int i;
i = 0x0F - which_LED;
GPIO_PORTF_DATA_R &= ~i; //turn off LED's which should not blink
GPIO_PORTF_DATA_R ^= which_LED;   //blink one LED on PF1 or 2 or 3
Delay();
}

