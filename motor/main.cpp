#include "mbed.h"
#include "Motor.h"
BusOut led(LED1, LED2, LED3, LED4);
Motor motor1(MOTOR1_IN1, MOTOR1_IN2, MOTOR1_PWM);
int main() {
 led = 0;
 float delta = 0.1f, i = 0.0f;
 while(1) {
 i += delta;
 if (fabs(i) >= 1.0f) delta *= -1.0f;
 printf("%f\r\n", i);
 motor1 = i;
 wait(0.5);
 }
}