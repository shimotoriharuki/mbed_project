#include "mbed.h"
BusOut led(LED1, LED2, LED3, LED4);
BusIn sw(SW1, SW2);
int main() {
 int state = -1;
 led = 0;
 sw.mode(PullUp);
 while(1) {
	if (state != sw) {
		if (sw & 0x01) printf("SW1 OFF ");
		else printf("SW1 ON ");
		if (sw & 0x02) printf("SW2 OFF\r\n");
		else printf("SW2 ON\r\n");
		state = sw;
	}	
 }
}
