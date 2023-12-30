#include "tm4c123gh6pm.h"
#include "tm4cstruct.h"
#include "stdio.h"
#include "uart0.h"
#include "adc0.h"
#include "ultrasonic.h"
#include "bluetooth.h"
#include "laser.h"
#include "magnetic_switch.h"

int main(void){
	__ASM("CPSIE I");
	
	NVIC->ISER[0] |= (1<<19);
	NVIC->ISER[0] |= (	1<<3);
	uart0_init();
	bluetooth_init();
	laser_init();
	adc0_init();
	int smoke_sensor_val;
	char smoke_sensor_message[50];
	magnetic_switch_init();
	laser_turn_on();
	while(1){
		
		Timer0ACapture_init();
		Measure_distance();
		smoke_sensor_val = get_val_adc0();
		sprintf(smoke_sensor_message, "\r\nValue = %d ", smoke_sensor_val);
		print_message_uart0(smoke_sensor_message);
		
		Delay(2000);
	}
	
	

	
}


