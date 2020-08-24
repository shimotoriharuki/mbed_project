#include "mbed.h"

//#define FOUR
//#define FIVE
//#define SIX
#define SEVEN
int main(){

#ifdef FOUR
	LPC_GPIO0->DIR |= 0x80;
	LPC_GPIO0->DATA |= 0x80;
	
#endif	
	
#ifdef FIVE	
	*(unsigned char *)(0x50028000) |= 0x08;	//DIR
	*(unsigned char *)(0x50023FFC) |= 0x08;	//DATA

#endif
	
#ifdef SIX
	LPC_GPIO0->DIR |= 0x08C;
	LPC_GPIO2->DIR |= 0x800;
	LPC_GPIO0->DATA &= 0x00;
	LPC_GPIO2->DATA &= 0x00;
	
	while(1){
		
		LPC_GPIO0->DATA &= 0x00;
		LPC_GPIO2->DATA |= 0x800;
		wait(0.5);
		

		LPC_GPIO0->DATA |= 0x04;
		LPC_GPIO2->DATA &= 0x00;
		
		wait(0.5);
	}
#endif	
	
#ifdef SEVEN
	LPC_GPIO0->DIR |= 0x08C;
	LPC_GPIO0->DATA &= 0x00;
	
	static double t = 0.001;	
	static double duty;
	static int flag;
	while(1){
		LPC_GPIO0->DATA &= 0x00;	//off
		wait(t - duty);
			
		LPC_GPIO0->DATA |= 0x04;	//on			
		wait(duty);
		
		
		if(duty >= 0.001){
			flag = 1;
		}
		else if(duty <= 0){
			flag = 0;
		}
			
		if(flag == 0){
			duty += 0.000001;
		}
		else if(flag == 1){
			duty -= 0.000001;
		}
			
		
		
		
	}
	
/*
volatile static int i = 0 ; // dummy counter
LPC_SYSCON->SYSAHBCLKCTRL |=0x100; // Timer16B1 Turn ON
LPC_IOCON->PIO0_3 = 0x001;	// PIO0_2 as CT16B1_MAT0
LPC_TMR16B1->PR = 7200-1;	// 10kHz (Max 16bit dec:65535)
LPC_TMR16B1->PWMC = 1;	// MAT0 as PWM
LPC_TMR16B1->MCR = 0x80;	// MR2 as Period of 16B1
LPC_TMR16B1->MR0 = 10000;	// PWM High duration=MR2-MR0
LPC_TMR16B1->MR2 = 15000;	// PWM Period (Max 16bit dec:65535)
LPC_TMR16B1->TCR = 2;	// TCR Reset
LPC_TMR16B1->TCR = 1;	// TCR Start

while(1) { // Infinite loop
	i++; // dummy count.
}
*/
#endif


	return 0;
}