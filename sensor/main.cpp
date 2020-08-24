#include "mbed.h"
BusOut led(LED1, LED2, LED3, LED4);
AnalogIn ad[] = {AD0};
PwmOut servo1(SERVO1);
int main() {
	float value;
	int target;
	led = 0;
	servo1.period_ms(20);
	while(1) {
		value = ad[0].read();
		
		target = 1000 + 1000 * value;
		printf("%f -> %d\r\n", value, target);
		servo1.pulsewidth_us(target);
		wait(0.01);
	}
}
