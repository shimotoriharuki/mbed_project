#include "mbed.h"
#include "Motor.h"
//#include "Sound.h"
Ticker flipper;

#define MAXPOWER 1
#define ROTATION_TIME 100

#define LIMITER
#define HARD
//#define EASY
//#define TEST

//prototype_declaration
void init(void);
void servo_val(void);
void led_led();

//pin setting
BusOut led(LED1, LED2, LED3, LED4);
BusIn start_sw(SW1, SW2);
AnalogIn distance[] = {AD0, AD1, AD2, AD3, AD5, AD7};
Motor motorR(MOTOR1_IN1, MOTOR1_IN2, MOTOR1_PWM);
Motor motorL(MOTOR2_IN1, MOTOR2_IN2, MOTOR2_PWM);
PwmOut servo1(SERVO4);
DigitalOut sw_led(P1_5);
DigitalIn red_sw(P3_2);

//global_declaration
int dis_flag = 0;
int timer1 = 0, timer2 = 0, timer3 = 0;
int sw_cnt = 0;
int level = 0;
int pattern = 5;

//initial_setting
void init(){

	servo1.period_ms(10);
	flipper.attach(*servo_val, 0.01);
	printf("program start\r\n");
	sw_led = 1;
	servo1.pulsewidth_us(2200);
	wait(1);
}


int main() {
	motorR.setMaxRatio(0.5);
	motorL.setMaxRatio(0.5);
	start_sw.mode(PullUp);
	red_sw.mode(PullUp);
	led = 0x00;
	
#ifdef TEST
	while(1){
		if(start_sw == 1){
			motorR = 1;
			motorL = 1;
		}
		else{
			motorR = 0;
			motorL = 0;
		}
		
	}
#endif
	
	while(start_sw != 2){
		if(start_sw == 1 && level == 0){
			level = 1;
			led = 0xff;
			printf("level = 1\r\n");
			wait(0.3);
		}
		else if(start_sw == 1 && level == 1){
			level = 0;
			led = 0x00;
			printf("level = 0\r\n");
			wait(0.3);
		}
	}
	init();

	double kp = 2, kd = 0, dt = 0.001;
	double dis0 = 0, dis1 = 0, dis2 = 0, dis3 = 0, dis4 = 0, dis5 = 0; 
	double devi1 = 0, devi2 = 0, devi3 = 0;
	double before_devi1 = 0, before_devi2 = 0, before_devi3 = 0;
	double cp = 0, cd = 0;
	double powerL = 0, powerR = 0;
	int sw_cnt = 0;
	int flag = 0;
	float maxpower = 0.5;
	
	
	
  while(1){
		switch(pattern){
			case 5:
				if(level == 1){
					maxpower = 1;
				}
				else{
					maxpower = 0.5;
				}
				
				pattern = 10;
			break;
		
			case 10:
				led_led();
			
				//stop
				if(start_sw == 1){
					pattern = 100;
					led = 0;
				}
	
				//red_sw push
				if(red_sw == 0){
					sw_cnt++;
				}
				else{
					sw_cnt = 0;
				}
				
				if(sw_cnt >= 50){
					pattern = 50;
					led = 0;
					sw_cnt = 0;
				}
				
				//red_sw on off
				if(timer2 <= 300){
					sw_led = 1;
					flag = 1;
				}
				else{
					sw_led = 0;
					flag = 0;
					if(timer2 >=600){
						timer2 = 0;
					}
				}
					
			//convert_to_distance	
				dis0 = (0.6 - distance[0].read()) * 3.0f;
				dis1 = (0.6 - distance[1].read()) * 3.0f;
				dis2 = (0.6 - distance[2].read()) * 3.0f;
				dis3 = (0.6 - distance[3].read()) * 3.0f;
				dis4 = (0.6 - distance[4].read()) * 3.0f;
				dis5 = (0.6 - distance[5].read()) * 3.0f;
				
			//switch gurd		
				if(dis4 < 0 || dis3 < 0){
					dis_flag = 1;
				}
				else if(timer1 > 300){
					dis_flag = 0;
				}
				
			//calculating_deviation
				devi1 = dis0 - dis1;
				devi2 = dis3 - dis2;
				devi3 = dis5 - dis4;
				
			//escape if	
				if((dis3 < 0 || dis4 < 0) && timer1 >= ROTATION_TIME){
					timer1 = 0;
					powerR = -maxpower;
					powerL = maxpower;
					motorR = powerR;
					motorL = powerL;
				}
				
				else if(dis2 < 0 && timer1 >= ROTATION_TIME){
					timer1 = 0;
					powerR = -maxpower;
					powerL = maxpower;
					motorR = powerR;
					motorL = powerL;
				}
				
				
				else if(dis5 < 0 && timer1 >= ROTATION_TIME){
					timer1 = 0;
					powerR = maxpower;
					powerL = -maxpower;			
					motorR = powerR;
					motorL = powerL;
				}
				
				else if(dis0 < 0 || dis1 < 0 ){
					cp = kp * devi1;
					cd = kd * (before_devi1 - devi1) / dt;
					powerR = (cp - cd);
					powerL = (cp - cd);			
					
				}
				
				else{
					powerL = powerR = 0;
				}
				
				if(timer1 >= ROTATION_TIME){
					motorR = powerR;
					motorL = powerL;
				}
				
				before_devi1 = devi1;
				before_devi2 = devi2;
				before_devi3 = devi3;
//				printf("timer = %d\r\n", timer1);
/*				
				printf("motorL = %f  ", powerL);
				printf("motorR = %f  \r\n", powerR);
*/					
/*					
				printf("dis0 = %f  ", dis0);
				printf("dis1 = %f  ", dis1);
				printf("dis2 = %f  ", dis2);
				printf("dis3 = %f  ", dis3);
				printf("dis4 = %f  ", dis4);
				printf("dis5 = %f  \r\n", dis5);
*/			
			/*
				printf("devi 0&1 = %f  ", devi1);
				printf("devi 2&3 = %f  ", devi2);
				printf("devi 4&5 = %f  \r\n", devi3);
			*/
		//		printf("\r\n \r\n");
				//wait(0.1);
			break;
			
			case 50:
				sw_led = 1;
				led = 0;
				printf("congratulations!\r\n");
				motorL = motorR = 0;
			
				if(start_sw == 1 && level == 0){
					level = 1;
					led = 0xff;
					printf("level = 1\r\n");
					wait(0.3);
				}
				else if(start_sw == 1 && level == 1){
					level = 0;
					led = 0x00;
					printf("level = 0\r\n");
					wait(0.3);
				}
				
				if(start_sw == 2){
					printf("start\r\n");
					pattern = 5;
					break;
				}
			break;
			
			case 100:
				printf("stop\r\n");

				while(1){
					//servo1.pulsewidth_us(2200);
					motorL = motorR = 0;
					
					if(start_sw == 1 && level == 0){
						level = 1;
						led = 0xff;
						printf("level = 1\r\n");
						wait(0.3);
					}
					else if(start_sw == 1 && level == 1){
						level = 0;
						led = 0x00;
						printf("level = 0\r\n");
						wait(0.3);
					}
					
					if(start_sw == 2){
						printf("start\r\n");
						pattern = 5;
						break;
					}
				}
			break;
					
		}
				
	}	
	
}



//servo
void servo_val(){
	
	timer1 += 10;	//time 10ms
	timer2 += 10;
	timer3 += 10;
	
	static int i = 2200;
	if(dis_flag == 1){
		i -= 30;
	//	printf("servo\r\n");
	}
	else i += 30;
	
	if(i >= 2200)
			i = 2200;
	if(i <= 1600)
			i = 1600;
	
	servo1.pulsewidth_us(i);
//	wait(0.01);
	
}

void led_led(){
	if(timer3 < 100){
		led = 1;
	}
	else if(timer3 < 200){
		led = 2;
	}
	else if(timer3 < 300){
		led = 4;
	}
	else{
		led = 8;
		timer3 = 0;
	}
	
}

