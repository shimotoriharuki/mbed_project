#include "mbed.h"
#include "Motor.h"
#include "QEI.h"

BusOut led(LED1,LED2,LED3,LED4);
BusIn sw(SW1, SW2);
BusIn in(GPIO1, GPIO2, GPIO3, GPIO4);
Motor motor_left(MOTOR1_IN1,MOTOR1_IN2,MOTOR1_PWM);
Motor motor_right(MOTOR2_IN1,MOTOR2_IN2,MOTOR2_PWM);
QEI qei_left(GPIO2, GPIO1, NC, 48, QEI::X4_ENCODING);
QEI qei_right(GPIO3, GPIO4, NC, 48, QEI::X4_ENCODING);
Ticker control;


/*----------Macro_Definition------------*/
#define low_pass_filter(x, x0, r) ((r)*(x) + (1.0 - (r))* (x0)) /* 0<= r <= 1 */

#define ONE
//#define TWO
//#define THREE
//#define FOUR
//#define FIVE


/*----------Gloval Variable-----------*/
double speed_left_ref = 0.1, speed_right_ref = 0.1;	//target_speed
double speed_left_lpf = 0.0, speed_right_lpf = 0.0;	//now_speed
const double sampling_time = 0.020;
const double move_per_pulse = 0.0005;	//mm/1puls -> 2*PI*r / 12
double i_left = 0.0, i_right = 0.0;
const double ki = 0.0, kp = 1.0;
int enc_left;		//enc_cnt
int enc_right;	//enc_cnt
double left_power, right_power;


/*-----------Functions---------------*/
void control_handler() {
	enc_left = qei_left.getPulses();
	enc_right = qei_right.getPulses();
	qei_left.reset();
	qei_right.reset();
	
	double speed_left = move_per_pulse * enc_left / sampling_time;
	double speed_right = move_per_pulse * enc_right / sampling_time;
	
	speed_left_lpf = low_pass_filter(speed_left, speed_left_lpf, 0.4);
	speed_right_lpf = low_pass_filter(speed_right, speed_right_lpf, 0.4);
	
	double delta_speed_left = speed_left_ref - speed_left_lpf;			//devi cal
	double delta_speed_right = -speed_right_ref - speed_right_lpf;	//devi cal
	
	i_left += delta_speed_left * sampling_time;			//integration
	i_right += delta_speed_right * sampling_time;		//integration
	
	left_power = -kp * delta_speed_left + ki * i_left;
	right_power = kp * delta_speed_right + ki * i_right;
	motor_left = -kp * delta_speed_left + ki * i_left;
	motor_right = kp * delta_speed_right + ki * i_right;
	
}

void init(){
	sw.mode(PullUp);
	in.mode(PullUp);
	while(1){
		if(sw == 2){
			wait(1);
			break;
		}
	}
	
	motor_left.setMaxRatio(1.0);
	motor_right.setMaxRatio(1.0);
	//control.attach(&control_handler, sampling_time);
	
}


/*---------------MAIN----------------*/
int main() {
	init();
	
	#ifdef ONE	
	float l= -1.0f;	//1:0.781530632[m/s]=x:0.2[m/s] -> x = 0.2*1 / 0.781530632
	float r= -1.0f;	
	wait(1);
	
	while(1) {
		int enc_left = qei_left.getPulses();
		int enc_right = qei_right.getPulses();
		qei_left.reset();
		qei_right.reset();
		double speed_left = move_per_pulse * enc_left / sampling_time;
		double speed_right = move_per_pulse * enc_right / sampling_time;
		
		printf("l = %f r = %f\r\n",speed_left,speed_right);
		
		motor_left = l;
		motor_right = r;
		wait(sampling_time);
	}
#endif
	
#ifdef TWO
	float l= -1.0f;	//1:0.781530632[m/s]=x:0.2[m/s] -> x = 0.2*1 / 0.781530632
	float r= -1.0f;	

	while(1) {
		int enc_left = qei_left.getPulses();
		int enc_right = qei_right.getPulses();
		qei_left.reset();
		qei_right.reset();
		double speed_left = move_per_pulse * enc_left / sampling_time;
		double speed_right = move_per_pulse * enc_right / sampling_time;
		
		printf("l = %f r = %f\r\n",speed_left,speed_right);	//target 0.2
		
		motor_left = l;
		motor_right = r;
		wait(sampling_time);
	}
#endif
	
#ifdef THREE
	while(1){
			int enc_left = qei_left.getPulses();
		int enc_right = qei_right.getPulses();

		qei_left.reset();
		qei_right.reset();
		printf("l = %d   r = %d\r\n", enc_left, enc_right);
		if(sw == 1) {
			control.detach();
			motor_left = motor_right = 0;
		}
	}
#endif
	
#ifdef FOUR
	
#endif
	
#ifdef FIVE
	
#endif
	
}