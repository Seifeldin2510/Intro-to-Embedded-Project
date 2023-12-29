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
	
	///Tesha8al el Ultrasonic
	uart0_init();
	uint32_t time;
	uint32_t distance;
	char mesg[20]; 
	while(1){

		sprintf(mesg, "\r\nDistance = %d cm", distance); /*convert float type distance data into string */
		print_message_uart0(mesg);
		Delay(2000);
		
	}
}

int get_distance(){
			Timer0ACapture_init();
		time = Measure_distance(); /* take pulse duration measurement */ 
		distance = (time * 10625)/10000000; /* convert pulse duration into distance */
}
