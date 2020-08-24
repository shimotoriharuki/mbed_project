#include "mbed.h"
#include "Motor.h"
#include "QEI.h"

#define DEBUG_MODE
#define DT 0.001

BusOut led(LED1, LED2, LED3, LED4);	
BusIn sw(SW1, SW2);
AnalogIn pen(AD7);									
Ticker pen_control;								
Serial pc(USBTX, USBRX);						
Motor motor_left(MOTOR1_IN1, MOTOR1_IN2, MOTOR1_PWM);		
Motor motor_right(MOTOR2_IN1, MOTOR2_IN2, MOTOR2_PWM);	

//**************************gloval variable**********************//
int pen_val;												
int goal_pen_val = 797;							
double power, last_speed;						
double pen_kp = 0.16, pen_kd = 0.00012, pen_ki = 0.001;
//double pen_kp = 0.16, pen_kd = 0, pen_ki = 0;
double pen_diff, last_pen_diff;	
double p, i, d;

//**************************functions**********************//710 890
void pen_control_handler(){
	pen_val = pen.read_u16() >> 6;					
	
	pen_diff = (double)(goal_pen_val - pen_val) ;	
	
	p = pen_kp * pen_diff;
	d = pen_kd * (last_pen_diff - pen_diff) / DT;
	i += pen_ki * pen_diff * DT;
	if(i >= 0.5)	i = 0.5;
	if(i <= -0.5)	i = -0.5;
	
	power = p - d + i;		
	
	if(power > 1.0) power = 1.0;
	if(power < -1.0) power = -1.0;
	
	if(power > 0.8 )	led = 1;
	else if(power <= 0.8 && power >= 0)	led = 2;
	else if(power < 0 && power >= -0.8)	led = 4;
	else if(power < -0.8)	led = 8;
	
	//last_speed = speed;										
	last_pen_diff = pen_diff;
	
	if(pen_val >= 870 || pen_val <= 720){
		motor_left = 0;
		motor_right = 0;
	}
	else{
		motor_left = power;
		motor_right = power;
	}

}

//*********************************main*****************************//
int main() {
	motor_left.setMaxRatio(0.5);
	motor_right.setMaxRatio(0.5);
	sw.mode(PullUp);	
	
	led = 1;		
	while(1)	if(sw == 2)	break;
	wait(0.5);		
	
	pen_control.attach(&pen_control_handler, DT);	//1ms	
	
	while(1) {	
		if(sw == 1) {
			pen_control.detach();
			motor_left = motor_right = 0;
		}
		
		#ifdef DEBUG_MODE	
		printf("pen:%d speed:%2.2f \r\n", pen_val ,power);
		wait(0.08);
		#endif
	}
}

