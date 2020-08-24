#include "mbed.h"

AnalogIn analog_value(P3_2);
PwmOut oto(P1_9);
DigitalOut led1(LED1);
Serial pc(USBTX,USBRX);
DigitalIn SW_IN(P3_3);      //??????

int main(){
    int meas=0;
    int flag = 0;
    int T=0;
    
    while(flag == 0){
        pc.printf("??????????\n\r");
        if(SW_IN == 0){
            flag = 1;
        }
    }
    
    while(flag == 1){
        meas = analog_value.read_u16();
        pc.printf("measure=%d mV\n\r",meas);
        oto.write(0.4);
        pc.printf("?????~\n\r");
        
        while(meas>=43000 &&  T!=1){             //?
            meas = analog_value.read_u16();
            oto.period(0.00191571);
            pc.printf("T=1\n\r");
            T=1;
        }while(meas<41000 && meas>=40000 &&  T!=2) {      //?
            meas = analog_value.read_u16();
            oto.period(0.00170648);
            pc.printf("T=2\n\r");
            T=2;
        }while(38000>meas && meas>=37000 && T!=3) {      //?
            meas = analog_value.read_u16();
            oto.period(0.00151976);
            pc.printf("T=\n\r");
            T=3;
        }while(35000>meas && meas>=34000 && T!=4) {      //??
            meas = analog_value.read_u16();
            oto.period(0.00143266);
            pc.printf("T=4\n\r");
            T=4;
        }while(32000>meas && meas>=31000 && T!=5) {      //?
            meas = analog_value.read_u16();
            oto.period(0.00127877);
            pc.printf("T=5\n\r");
            T=5;
        }while(29000>meas && meas>=28000 && T!=6) {      //?
            meas = analog_value.read_u16();
            oto.period(0.00113636);
            pc.printf("T=6\n\r");
            T=6;
        }while(26000>meas && meas>=25000 && T!=7) {      //?
            meas = analog_value.read_u16();
            oto.period(0.00101215);
            pc.printf("T=7\n\r");
            T=7;
        }while(23000>meas && meas>=22000 && T!=8) {      //?2
            meas = analog_value.read_u16();
            oto.period(0.00095602);
            pc.printf("T=8\n\r");
            T=8;
        }while(meas<20000 && T!=9){
            meas = analog_value.read_u16();
            oto.write(0);
            pc.printf("T=9\n\r");
            T=9;
        }//while
    }//while2
} //main