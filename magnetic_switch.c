#include "tm4cstruct.h"
#include "magnetic_switch.h"
#include "laser.h"
#include "uart0.h"
#include "stdbool.h"

extern bool MAGNETIC_STATE = false;

void magnetic_switch_init(void){
		 SYSCTL->RCGCGPIO |= (1<<3);   /* Set bit5 of RCGCGPIO to enable clock to PORTF*/
		 while(SYSCTL->RCGCGPIO & (1<<3) == 0);


    /*Initialize PF3 as a digital output, PF0 and PF4 as digital input pins */
	
    GPIOD->DIR &= ~1<<0;  /* Set PF4 and PF0 as a digital input pins */
    GPIOD->DEN |= 1<<0;             /* make PORTF4-0 digital pins */
    GPIOD->PUR |= 1<<0;             /* enable pull up for PORTF4, 0 */
    
    /* configure PORTF4, 0 for falling edge trigger interrupt */
    GPIOD->IS  &= ~(1<<0);        /* make bit 4, 0 edge sensitive */
    GPIOD->IBE &= ~(1<<0);         /* trigger is controlled by IEV */
    GPIOD->IEV &= ~(1<<0);        /* falling edge trigger */
    GPIOD->ICR |= (1<<0);          /* clear any prior interrupt */
    GPIOD->IM  |= (1<<0);          /* unmask interrupt */

}

void GPIOD_Handler(void){
	//laser_turn_on('m');	
	MAGNETIC_STATE = true;
	GPIOD->ICR = 1;	
		//print_message_uart0("SUIII");
		
}