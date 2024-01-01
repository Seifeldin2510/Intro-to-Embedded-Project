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
	
	  /* Given 10us trigger pulse */
	  GPIOA->DATA &= ~(1<<4); /* make trigger  pin high */
	  Delay_MicroSecond(10); /*10 seconds delay */
	  GPIOA->DATA |= (1<<4); /* make trigger  pin high */
	  Delay_MicroSecond(10); /*10 seconds delay */
	  GPIOA->DATA &= ~(1<<4); /* make trigger  pin low */

 	while(1)
	{
    TIMER0->ICR = 4;            /* clear timer0A capture flag */
    while((TIMER0->RIS & 4) == 0) ;    /* wait till captured */
	  if(GPIOB->DATA & (1<<6)) /*check if rising edge occurs */
		{
    lastEdge = TIMER0->TAR;     /* save the timestamp */
		/* detect falling edge */
    TIMER0->ICR = 4;            /* clear timer0A capture flag */
    while((TIMER0->RIS & 4) == 0) ;    /* wait till captured */
    thisEdge = TIMER0->TAR;     /* save the timestamp */
		return (thisEdge - lastEdge); /* return the time difference */
		}
	}
}

/* Timer0A initialization function */
/* Initialize Timer0A in input-edge time mode with up-count mode */
void Timer0ACapture_init(void)
{
    SYSCTL->RCGCTIMER |= 1;     /* enable clock to Timer Block 0 */
    SYSCTL->RCGCGPIO |= 2;      /* enable clock to PORTB */
    
    GPIOB->DIR &= ~0x40;        /* make PB6 an input pin */
    GPIOB->DEN |= 0x40;         /* make PB6 as digital pin */
    GPIOB->AFSEL |= 0x40;       /* use PB6 alternate function */
    GPIOB->PCTL &= ~0x0F000000;  /* configure PB6 for T0CCP0 */
    GPIOB->PCTL |= 0x07000000;
    
	  /* PB2 as a digital output signal to provide trigger signal */
	  SYSCTL->RCGCGPIO |= 1;      /* enable clock to PORTA */
	  GPIOA->DIR |=(1<<4);         /* set PB2 as a digial output pin */
	  GPIOA->DEN |=(1<<4);         /* make PB2 as digital pin */

    TIMER0->CTL &= ~1;          /* disable timer0A during setup */
    TIMER0->CFG = 4;            /* 16-bit timer mode */
    TIMER0->TAMR = 0x17;        /* up-count, edge-time, capture mode */
    TIMER0->CTL |=0x0C;        /* capture the rising edge */
    TIMER0->CTL |= (1<<0);           /* enable timer0A */
}



/* Create one microsecond second delay using Timer block 1 and sub timer A */

void Delay_MicroSecond(int time)
{
    int i;
    SYSCTL->RCGCTIMER |= 2;     /* enable clock to Timer Block 1 */
    TIMER1->CTL = 0;            /* disable Timer before initialization */
    TIMER1->CFG = 0x04;         /* 16-bit option */ 
    TIMER1->TAMR = 0x02;        /* periodic mode and down-counter */
    TIMER1->TAILR = 16 - 1;  /* TimerA interval load value reg */
    TIMER1->ICR = 0x1;          /* clear the TimerA timeout flag */
    TIMER1->CTL |= 0x01;        /* enable Timer A after initialization */

    for(i = 0; i < time; i++)
    {
        while ((TIMER1->RIS & 0x1) == 0) ;      /* wait for TimerA timeout flag */
        TIMER1->ICR = 0x1;      /* clear the TimerA timeout flag */
    }
}


void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter*1000; i++);
}