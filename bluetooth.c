#include "tm4cstruct.h"
#include "bluetooth.h"


void bluetooth_init(void){
	//Sha8al clock UART5
	SYSCTL->RCGCUART |= 0x20;
	while(SYSCTL->RCGCUART & 0x20 == 0);
	//E3mel disable lel UART5
	UART5->CTL = 0x0;
	
	//sha8al el clock beta3et port E
	SYSCTL->RCGCGPIO |= 0x10;
	while(SYSCTL->RCGCGPIO & 0x10 == 0);
	UART5->CC = 0;
	//Sha8al Port E
	GPIOE->DEN = 0x30;
	
	//Esta5dem Alternate Function beta3 pin 0 w 1 f port E
	GPIOE->AFSEL = 0x30;
	
	//Esta5dem el alternate function beta3et el UART
	GPIOE->PCTL = 0x00110000;
	
	//El integer part beta3 el baud divisor
	UART5->IBRD = 104;
	
	//El fraction part beta3 el baud divisor
	//(fraction part * 64) + 0.5
	UART5->FBRD = 11;
	
	//Sha8al el UART0 w el RX w el TX
	UART5->CTL = 0x301;
	
	//Zabat el control register, 5ali el data b 8 bits, sha8al el FIFO 3ashan Seif, 1 stop bit w mn 8eer parity
	UART5->LCRH = 0x60;
}

void bluetooth_write(char c){
		//Et2aked en el buffer msh full
		while((UART5->FR & (1<<5)) != 0);
		
		//Eb3at el data
		UART5->DR = c;
}

void bluetooth_write_message(char* message){
	while(*message){
		bluetooth_write(*(message++));
	}
}

char bluetooth_read(void)  
{
    char data;
	  while((UART5->FR & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART5->DR;  	/* before giving it another byte */
    return (unsigned char) data; 
}

