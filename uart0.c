#include "uart0.h"
#include "tm4cstruct.h"


void uart0_init(void){
	//Sha8al clock UART0
	SYSCTL->RCGCUART |= 0x1;
	while(SYSCTL->RCGCUART & 0x1 == 0);
	//E3mel disable lel UART0
	UART0->CTL = 0x0;
	
	//sha8al el clock beta3et port A
	SYSCTL->RCGCGPIO |= 0x1;
	while(SYSCTL->RCGCGPIO & 0x1 == 0);
	
	//Sha8al Port A
	GPIOA->DEN = 0x03;
	
	//Esta5dem Alternate Function beta3 pin 0 w 1 f port A
	GPIOA->AFSEL = 0x03;
	
	//Esta5dem el alternate function beta3et el UART
	GPIOA->PCTL = 0x11;
	
	//El integer part beta3 el baud divisor
	UART0->IBRD = 104;
	
	//El fraction part beta3 el baud divisor
	//(fraction part * 64) + 0.5
	UART0->FBRD = 11;
	
	//Sha8al el UART0 w el RX w el TX
	UART0->CTL = 0x301;
	
	//Zabat el control register, 5ali el data b 8 bits, sha8al el FIFO 3ashan Seif, 1 stop bit w mn 8eer parity
	UART0->LCRH = 0x70;
	
}

void send_message_uart0(char message){
		//Et2aked en el buffer msh full
		while((UART0->FR & 0X20) != 0);
		
		//Eb3at el data
		UART0->DR = message;

}

void print_message_uart0(char *str){
	while(*str){
		send_message_uart0(*(str++));
	}
}
