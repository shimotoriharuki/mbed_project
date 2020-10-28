#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>

#define PI 3.1415926
#define L 100.0    /* mm  */
#define dt 0.02   /* sec */

#define RAD *PI/180
#define DEG *180/PI

int main(){
	FILE *fp1, *fp2;
	int i;
	int flag = 0;
	double v_right, v_left;
	double v, w;
	double vx, vy;
	double x0 = 0, y0 = 0, tht0 = 0 DEG;
	double x1 = 0, y1 = 0, tht1 = 0;
	double dx = 0, dy = 0, dtht = 0;
	double dx0 = 0, dy0 = 0, dtht0 = 0;
	double sin_tht, cos_tht;
	
	if((fp1 = fopen("Data_cv.csv", "r"))==NULL){
		fprintf(stderr, "Can not find Data_cv.csv\n");
		exit(1);
	}
	if((fp2 = fopen("DataVelo.txt","w"))==NULL){
		fprintf(stderr, "Can not find DataPath_cv.txt\n");
		exit(1);
	}
	
	//x0 = 0.0;   y0 = 0.0;	tht0 = 0;
	//x0 = 0.0;   y0 = 0.0;	tht0 = 0.0;
	
	while(fscanf(fp1, "%lf %lf", &x1, &y1) != EOF){
		//v_left = 0;
		//v_right = 0;
		dx = x1 - x0;
		dy = y1 - y0;
		
		vx = dx / dt;
		vy = dy / dt;
		vx = vx * vx;	//square
		vy = vy * vy;	//square
		v = sqrt(vx + vy);
		
		
		
		if(dy > 0 && dx == 0)	tht1 = 90 RAD;
		else if(dy < 0 && dx == 0)	tht1 = -90 RAD;
		else if(dy == 0 && dx == 0)	tht1 = 0;
		else	tht1 = atan(dy / dx);
		
		/*
		if(dx < 0 && dy > 0){	//2
			tht1 *= -1;
			tht1 = 180 RAD - tht1;
		}
		else if(dx < 0 && dy < 0){	//3
			tht1 = 180 RAD + tht1;
		}
		else if(dy < 0 && dx > 0){	//4	
			
			if(dx0 > 0 && dy0 > 0){
				tht0 *= -1;
			}
			
			tht1 *= -1;	
			tht1 = 360 RAD - tht1;
			
		}
		*/
		/*
		if(dx < 0 ){
			tht1 *= -1;
			tht1 = 180 RAD - tht1;
			
		}
		*/
		if(dy0 * dy < 0 && dy0 > 0){
			tht1 = 360 RAD + tht1;
			w = -1*(tht1 - tht0) / dt;
			std::cout  << "cc" << std::endl;
			
		}
		else if(dy0 * dy < 0 && dy0 < 0){
			tht0 = 360 RAD - tht0;
			w = (tht1 + tht0) / dt;
			std::cout  << "bb" << std::endl;
			
		}
		else if(dx > 0 && dy < 0){	//4
			tht1 *= -1;
			tht1 = 360 RAD - tht1;
			w = (tht1 - tht0) / dt;
			std::cout  << "aa" << std::endl;
		}
		else if(dx < 0 && dy > 0){	//2
			tht1 *= -1;
			tht1 = 180 RAD - tht1;
			w = (tht1 - tht0) / dt;
			std::cout  << "dd" << std::endl;
		}
		else{	//1
			w = (tht1 - tht0) / dt;
			std::cout  << "ee" << std::endl;
		}
		
		
		
		/*
		if(dx == dx0 && dy == dy0)	tht1 = 0;
		else if(dx > dy)	tht1 = -(tht0 - tht1);		
		else if(dy > dx){
			if(tht0 < 0)	tht1 = tht1 + tht0;
			else			tht1 = tht1 - tht0;
		}
		*/
		
		
		std::cout  << tht1*180/PI << std::endl;
		
		
		
		v_left = (2 * v - L * w) / 2;
		v_right = (2 * v + L * w) / 2;
		
		fprintf(fp2, "%lf\t%lf\n", v_right, v_left);
		//fprintf(fp2, "%lf\t%lf\t%lf\n", v_right, v_left, tht1 * 180 / PI);
		
		
		x0 = x1;    y0 = y1;	tht0 = tht1;
		dx0 = dx;	dy0 = dy;	dtht0 = dtht;
		
	}
	fclose(fp1);
	fclose(fp2);
	printf("機体の座標データから左右の車輪の速度を計算し、DataVelo.txtに結果をセーブした\n");
	return 0;
}
