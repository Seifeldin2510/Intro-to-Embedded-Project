#include "tm4cstruct.h"
#include "laser.h"

void laser_init(){
	SYSCTL->RCGCGPIO |= 1;      
	while(SYSCTL->RCGCGPIO & 1 == 0);
	GPIOA->DEN |= (1<<5);  
	GPIOA->DIR |= (1<<5);        
}

void laser_turn_on(){
	SysTick->LOAD = 15999999; // one second delay relaod value
	SysTick->CTRL = 7 ; // enable counter, interrupt and select system bus clock 
	SysTick->VAL  = 0; //initialize current value register 
	GPIOA->DATA = (1<<5);

}
void SysTick_Handler(void){
	GPIOA->DATA = ~(1<<5);
}