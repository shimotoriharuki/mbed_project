#include "mbed.h"

BusOut led(LED1, LED2, LED3, LED4);


int main() {
  while(1){
    led = 1;
    wait(0.5);
    led = 0;
    wait(0.5);
		printf("The setup is complete!\r\n");
  }
}
