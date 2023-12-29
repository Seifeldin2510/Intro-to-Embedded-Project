
/*
Steps:
- Enable UART5
- write and read data
*/


void bluetooth_init(void);
void bluetooth_write(char c);
void bluetooth_write_message(char *message);
char bluetooth_read(void);