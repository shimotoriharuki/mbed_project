//Maximum path is 1000(Variable type is double) 1m40s
//Maximum path is 2000(Variable type is int) 3m20s

#include "mbed.h"
#include "Motor.h"
#include "QEI.h"
#include "./path_data.h"

BusOut led(LED1,LED2,LED3,LED4);
BusIn sw(SW1, SW2);
BusIn in(GPIO1, GPIO2, GPIO3, GPIO4);
Motor motor_left(MOTOR1_IN1,MOTOR1_IN2,MOTOR1_PWM);
Motor motor_right(MOTOR2_IN1,MOTOR2_IN2,MOTOR2_PWM);
QEI qei_left(GPIO2, GPIO1, NC, 48, QEI::X4_ENCODING);
QEI qei_right(GPIO4, GPIO3, NC, 48, QEI::X4_ENCODING);
Ticker control;
Ticker timer;

/*----------Macro_Definition------------*/
#define low_pass_filter(x, x0, r) ((r)*(x) + (1.0 - (r))* (x0)) /* 0<= r <= 1 */

//#define ONE
//#define TWO
#define THREE
//#define FOUR
//#define FIVE


/*----------Gloval Variable-----------*/
double speed_left_ref = 0.0, speed_right_ref = 0.0;	//target_speed
double speed_left_lpf = 0.0, speed_right_lpf = 0.0;	//now_speed
const double sampling_time = 0.020;	//20ms
const double move_per_pulse = 0.0005;	//mm/1puls -> 2*PI*r / 12 (1?)
double i_left = 0.0, i_right = 0.0;
const double ki = 30, kp = 4.64;	//maxpgain 5.8
int enc_left;		//enc_cnt
int enc_right;	//enc_cnt
double left_power, right_power;
int t_cnt = 0;
int vpath_idx = 0;
double timer1;

/*-----------Functions---------------*/
void tim_flip(){	//1[ms]timer
	timer1++;
}

void control_handler() {
	enc_left = qei_left.getPulses();
	enc_right = qei_right.getPulses();
	qei_left.reset();
	qei_right.reset();
	
	double speed_left = move_per_pulse * enc_left / sampling_time;		//now_velo
	double speed_right = move_per_pulse * enc_right / sampling_time;	//now_velo
	
	speed_left_lpf = low_pass_filter(speed_left, speed_left_lpf, 0.4);
	speed_right_lpf = low_pass_filter(speed_right, speed_right_lpf, 0.4);

	if (t_cnt==5){ //100[do/ms]
		vpath_idx++; 
		t_cnt = 0;
	}
	if (vpath_idx < MaxNum ){
		speed_right_ref = velo_data[vpath_idx*2]/1000.0; 
		speed_left_ref = velo_data[vpath_idx*2+1] /1000.0; 
	}
	else{	//path_data finish
		speed_right_ref = 0.0;
		speed_left_ref = 0.0;
		motor_left = motor_right = 0;
	}

	double delta_speed_left = speed_left_ref - speed_left_lpf;			//devi cal
	double delta_speed_right = -speed_right_ref - speed_right_lpf;	//devi cal
	
	i_left -= delta_speed_left * sampling_time;			//integration
	i_right += delta_speed_right * sampling_time;		//integration
	
	//left_power = -kp * delta_speed_left + ki * i_left;
	//right_power = kp * delta_speed_right + ki * i_right;
	motor_left = -kp * delta_speed_left + ki * i_left;
	motor_right = kp * delta_speed_right + ki * i_right;
	
	t_cnt++;
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
	control.attach(&control_handler, sampling_time);	//counter timer
	//timer.attach(&tim_flip, 0.001);
	
}


/*---------------MAIN----------------*/
int main() {
	init();
	while(1){
		//printf("l = %lf   r = %lf\r\n", i_left, i_right);
		if(sw == 1) {
			control.detach();
			motor_left = motor_right = 0;
		}
	}
	
}