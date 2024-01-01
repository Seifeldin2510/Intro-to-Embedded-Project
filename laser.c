#include "tm4cstruct.h"
#include "tm4c123gh6pm.h"
#include "laser.h"
#include "stdbool.h"


void laser_init(){
	SYSCTL->RCGCGPIO |= 1;      
	while(SYSCTL->RCGCGPIO & 1 == 0);
	GPIOA->DEN |= (1<<5);  
	GPIOA->DIR |= (1<<5);   
	GPIOA->DATA &= ~(1<<5);	
}

void laser_turn_on(char state){
	//u -> ultrasonic 1100 -> 500 ms
	//f -> fume 1110 -> 750 ms
	//m -> magnetic 1111 -> 1000 ms

	uint32_t x;
	switch(state){
		case 'u':
				GPIOA->DATA |= (1<<5);
				SysTick->LOAD = (8000000) - 1; 
				SysTick->CTRL = 5 ; 
				SysTick->VAL  = 0; 
				while(!(SysTick->CTRL & 0x10000));
				x = SysTick->VAL;
				GPIOA->DATA &= ~(1<<5);

				SysTick->CTRL = 0; 
		    break;
		case 'f':
				GPIOA->DATA |= (1<<5);
				SysTick->LOAD = (12000000) - 1; 
				SysTick->CTRL = 5 ; 
				SysTick->VAL  = 0; 
				while(!(SysTick->CTRL & 0x10000));
				x = SysTick->VAL;
				GPIOA->DATA &= ~(1<<5);

				SysTick->CTRL = 0; 
		    break;
		case 'm':
				GPIOA->DATA |= (1<<5);
				SysTick->LOAD = (16000000) - 1; 
				SysTick->CTRL = 5 ; 
				SysTick->VAL  = 0; 
				while(!(SysTick->CTRL & 0x10000));
				x = SysTick->VAL;
				GPIOA->DATA &= ~(1<<5);
				SysTick->CTRL = 0; 
		    break;
	}

	

}