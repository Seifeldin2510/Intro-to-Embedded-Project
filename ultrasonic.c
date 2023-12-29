#include "ultrasonic.h"
#include "uart0.h"
#include "stdint.h"
#include "stdbool.h"
#include "bluetooth.h"
#include "tm4cstruct.h"


int LAST_EDGE;
bool DA5AL_ABL_KEDA;


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
		TIMER0->IMR |=(1<<2);
    TIMER0->CTL |= (1<<0);           /* enable timer0A */
		
}

/* Create one microsecond second delay using Timer block 1 and sub timer A */

void Delay_MicroSecond(unsigned long time)
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

void Measure_distance(void)
{
    int lastEdge, thisEdge;
		DA5AL_ABL_KEDA = false;
	  /* Given 10us trigger pulse */
	  GPIOA->DATA &= ~(1<<4); /* make trigger  pin high */
	  Delay_MicroSecond(10); /*10 seconds delay */
	  GPIOA->DATA |= (1<<4); /* make trigger  pin high */
	  Delay_MicroSecond(10); /*10 seconds delay */
	  GPIOA->DATA &= ~(1<<4); /* make trigger  pin low */
}


void TIMER0A_Handler(){
		if(!DA5AL_ABL_KEDA){
			LAST_EDGE = TIMER0->TAR;
			DA5AL_ABL_KEDA = true;
			TIMER0->ICR = 4;
		}else{
			int thisEdge = TIMER0->TAR;
			uint32_t diff = thisEdge - LAST_EDGE;
			int distance = (1062.5 * diff) / 1000000; 
			char message[20];
			sprintf(message, "\r\nDistance = %d cm", distance); 
			bluetooth_write_message(message);
			print_message_uart0(message);
			TIMER0->ICR = 4;
			DA5AL_ABL_KEDA = false;
			
	}


}


