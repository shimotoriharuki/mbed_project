// Chiba Institute of Technology

#ifndef MBED_MOTOR_H
#define MBED_MOTOR_H

#include "mbed.h"
#include "SoftwarePWM.h"

/** Class to control a motor on any pin, without using pwm pin
 *
 * Example:
 * @code
 * // Motor Control
 * #include "mbed.h"
 * #include "Motor.h"
 *
 * Motor motor(xp31, xp32, xp33);
 * Motor motor(xp34, xp35, xp36);
 *
 * int main(){
 * while(1) {
 *     for (int pos = 1000; pos < 2000; pos += 25) {
 *         Servo1.SetPosition(pos);
 *         wait_ms(20);
 *     }
 *     for (int pos = 2000; pos > 1000; pos -= 25) {
 *         Servo1.SetPosition(pos);
 *         wait_ms(20);
 *     }
 * }
 * @endcode
 */


class Motor
{
public:
    /** Create a new SoftwarePWM object on any mbed pin
      *
      * @param Pin Pin on mbed to connect PWM device to
     */
    Motor(PinName Pa0, PinName Pa1, PinName Ppwm);

		void setMaxRatio(float max_ratio);

    void period(float seconds);

    void write(float value);

    float read();
    
#ifdef MBED_OPERATORS
    /** A operator shorthand for write()
     */
    Motor& operator= (float value) {
        write(value);
        return *this;
    }

    Motor& operator= (Motor& rhs) {
        write(rhs.read());
        return *this;
    }

    /** An operator shorthand for read()
     */
    operator float() {
        return read();
    }
#endif

private:
		SoftwarePWM SoftwarePWMPin;
		DigitalOut a0;
		DigitalOut a1;
    float value;
    float period_us;
		float max_ratio;
};

#endif
