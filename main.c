#include "tm4c123gh6pm.h"
#include "tm4cstruct.h"
#include "stdio.h"
#include "uart0.h"
#include "adc0.h"
#include "ultrasonic.h"
#include "bluetooth.h"

int main(void){
	//Tesha8al el Smoke Sensor
	/*int smoke_sensor_val;
	char smoke_sensor_message[50];
	uart0_init();
	adc0_init();
	print_message_uart0("Tiva Started\n");
	while(1){
		smoke_sensor_val = get_val_adc0();
		sprintf(smoke_sensor_message, "\r\nValue = %d ", smoke_sensor_val);
		print_message_uart0(smoke_sensor_message);
	}*/
	__ASM("CPSIE I");
	NVIC->ISER[0] |= (1<<19);
	///Tesha8al el Ultrasonic
	uart0_init();
	bluetooth_init();
	while(1){
		Timer0ACapture_init();
		Measure_distance();
		Delay(2000);
	}
	
	

	
}


