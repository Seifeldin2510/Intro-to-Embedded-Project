#include "tm4cstruct.h"
#include "magnetic_switch.h"
#include "laser.h"
#include "uart0.h"
#include "stdbool.h"

extern bool MAGNETIC_STATE = false;

void magnetic_switch_init(void){
		 SYSCTL->RCGCGPIO |= (1<<3);   
		 while(SYSCTL->RCGCGPIO & (1<<3) == 0);


	
    GPIOD->DIR &= ~1<<0;  
    GPIOD->DEN |= 1<<0;             
    GPIOD->PUR |= 1<<0;             
    
    
    GPIOD->IS  &= ~(1<<0);        
    GPIOD->IBE &= ~(1<<0);        
    GPIOD->IEV &= ~(1<<0);        
    GPIOD->ICR |= (1<<0);          
    GPIOD->IM  |= (1<<0);          

}

void GPIOD_Handler(void){
	
	MAGNETIC_STATE = true;
	GPIOD->ICR = 1;	
		
		
}