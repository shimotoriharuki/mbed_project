#include "mbed.h"
BusOut led(LED1, LED2, LED3, LED4);
PwmOut servo1(SERVO1);
int main() {
 led = 0;
 servo1.period_ms(20);
 while(1) {
 for(int i = 1000; i <= 2000; i += 50){
 printf("%d\r\n", i);
 servo1.pulsewidth_us(i);
 wait(0.1);
 }
 }
}