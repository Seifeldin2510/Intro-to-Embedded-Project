#include "adc0.h"
#include "stdbool.h"
#include "tm4cstruct.h"

void adc0_init(){
	
    
    SYSCTL->RCGCGPIO |= (1<<4);   
    SYSCTL->RCGCADC |= (1<<0);    
    
    
    GPIOE->AFSEL |= (1<<3);       
    GPIOE->DEN &= ~(1<<3);        
    GPIOE->AMSEL |= (1<<3);       
   
    
    ADC0->ACTSS &= ~(1<<3);        
    ADC0->EMUX &= ~0xF000;    
    ADC0->SSMUX3 = 0;         
    ADC0->SSCTL3 |= (1<<1)|(1<<2); 
    ADC0->ACTSS |= (1<<3);        
}


int get_val_adc0(){
		unsigned int adc_value;
    ADC0->PSSI |= (1<<3);        
    while((ADC0->RIS & 8) == 0) ;   
    adc_value = ADC0->SSFIFO3; 
    ADC0->ISC = 8; 
		return adc_value;	
}