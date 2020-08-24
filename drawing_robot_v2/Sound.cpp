#include "Sound.h"
#include "mbed.h"

Sound::Sound(PinName Ppwm)
  : SoftwarePWMPin(Ppwm), period(0.0f)
{
	this->write(0);
	SoftwarePWMPin.Enable(0, 0);
}

void Sound::setPeriod(float seconds)
{
	int period_us = (int)(1000000.0f / frequency);
	SoftwarePWMPin.Enable(0, period_us);
}

void Sound::write(float frequency)
{
	this->frequency = frequency;
	if (frequency != 0){
		int us = (int)(1000000.0f / frequency);
		SoftwarePWMPin.Disable();
		SoftwarePWMPin.Enable(us/2, us);
	} else {
		SoftwarePWMPin.Disable();
	}
}

float Sound::read()
{
  return frequency;
}
