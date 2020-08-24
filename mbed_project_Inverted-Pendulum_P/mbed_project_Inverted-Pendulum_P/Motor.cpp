#include "Motor.h"
#include "mbed.h"

Motor::Motor(PinName Pa0, PinName Pa1, PinName Ppwm)
  : SoftwarePWMPin(Ppwm), a0(Pa0), a1(Pa1), max_ratio(0.25)
{
	this->period(0.0005);		//モータドライバのスイッチング周波数を2kHzに設定
	this->write(0);
}

void Motor::setMaxRatio(float max_ratio)
{
	this->max_ratio = max_ratio;
}


void Motor::period(float seconds)
{
	period_us = (int)(seconds * 1000000);
	SoftwarePWMPin.Enable(0, period_us);
}

void Motor::write(float value)
{
	if (value > 1.0f) value = 1.0f;
	if (value < -1.0f) value = -1.0f;
	this->value = value;
	if (value > 0) {
		a0 = 1, a1 = 0;
	} else {
		a0 = 0, a1 = 1;
	}
	int us = (int)((float)period_us * fabs(value * max_ratio));
	SoftwarePWMPin.SetPosition(us);
}

float Motor::read()
{
  return value;
}

	
	
