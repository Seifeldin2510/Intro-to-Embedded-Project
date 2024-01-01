#include "stdbool.h"
#include "stdint.h"
#include "tm4cstruct.h"
#include "tm4c123gh6pm.h"
#include "uart0.h"
#include "adc0.h"
#include "laser.h"
#include "bluetooth.h"
#include "magnetic_switch.h"





void portf_init();
uint32_t Measure_distance(void);
void Timer0ACapture_init(void);
void Delay_MicroSecond(int time);
void Delay(unsigned long counter);


uint32_t time; 
uint32_t distance; 
char ultrasonic_mesg[20];  
char smoke_mesg[20];
uint32_t smoke_val;
bool started = false;
int start_status = 1;

int main(void)
{
	portf_init();
	uart0_init();
	uart0_init();
	adc0_init();
	laser_init();
	bluetooth_init();
	portf_init();
	Timer0ACapture_init(); 
	while(1){
		smoke_val = get_val_adc0();
		time = Measure_distance(); 
		distance = (time * 10625)/10000000; 
		sprintf(ultrasonic_mesg, "\r\nDistance = %d cm", distance);
		sprintf(smoke_mesg, "\r\nSmoke Value: %d", smoke_val);
		print_message_uart0(ultrasonic_mesg);
		print_message_uart0(smoke_mesg);
		if(distance < 20){
			laser_turn_on('u');
			bluetooth_write_message("\r\nAn intruder is detected");
		}else if(smoke_val > 1000){
			laser_turn_on('f');
			bluetooth_write_message("\r\nFire");
		}
		if((GPIOF->DATA & (1<<0)) == 0){
			started = false;
		}
		Delay(2000);
	}
}



void portf_init(){
	  SYSCTL_RCGCGPIO_R |= 0x00000020;
    while((SYSCTL_PRGPIO_R & 0x00000020) == 0){};
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;
}



uint32_t Measure_distance(void)
{
    int lastEdge, thisEdge;
	
	  
	  GPIOA->DATA &= ~(1<<4);
	  Delay_MicroSecond(10); 
	  GPIOA->DATA |= (1<<4); 
	  Delay_MicroSecond(10); 
	  GPIOA->DATA &= ~(1<<4); 

 	while(1)
	{
    TIMER0->ICR = 4;            
    while((TIMER0->RIS & 4) == 0) ;    
	  if(GPIOB->DATA & (1<<6)) 
		{
    lastEdge = TIMER0->TAR;     
		
    TIMER0->ICR = 4;            
    while((TIMER0->RIS & 4) == 0) ;    
    thisEdge = TIMER0->TAR;     
		return (thisEdge - lastEdge); 
		}
	}
}


void Timer0ACapture_init(void)
{
    SYSCTL->RCGCTIMER |= 1;     
    SYSCTL->RCGCGPIO |= 2;      
    
    GPIOB->DIR &= ~0x40;        
    GPIOB->DEN |= 0x40;         
    GPIOB->AFSEL |= 0x40;       
    GPIOB->PCTL &= ~0x0F000000;  
    GPIOB->PCTL |= 0x07000000;
    

	  SYSCTL->RCGCGPIO |= 1;     
	  GPIOA->DIR |=(1<<4);         
	  GPIOA->DEN |=(1<<4);         

    TIMER0->CTL &= ~1;         
    TIMER0->CFG = 4;           
    TIMER0->TAMR = 0x17;       
    TIMER0->CTL |=0x0C;        
    TIMER0->CTL |= (1<<0);           
}





void Delay_MicroSecond(int time)
{
    int i;
    SYSCTL->RCGCTIMER |= 2;  
    TIMER1->CTL = 0;         
    TIMER1->CFG = 0x04;      
    TIMER1->TAMR = 0x02;     
    TIMER1->TAILR = 16 - 1;  
    TIMER1->ICR = 0x1;          
    TIMER1->CTL |= 0x01;        

    for(i = 0; i < time; i++)
    {
        while ((TIMER1->RIS & 0x1) == 0) ;
        TIMER1->ICR = 0x1;      
    }
}


void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter*1000; i++);
}