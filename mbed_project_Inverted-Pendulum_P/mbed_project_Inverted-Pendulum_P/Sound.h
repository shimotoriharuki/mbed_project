// Chiba Institute of Technology

#ifndef MBED_SOUND_H
#define MBED_SOUND_H

#include "mbed.h"
#include "SoftwarePWM.h"

/** Class to control a motor on any pin, without using pwm pin
 *
 * Example:
 * @code
 * // Motor Control
 * #include "mbed.h"
 * #include "Sound.h"
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

typedef enum{
	C2 = 130,
	D2 = 146,
	E2 = 164,
	F2 = 174,
	G2 = 195,
	A3 = 220,
	B3 = 246,
	C4 = 261,
	D4 = 293,
	E4 = 329,
	F4 = 349,
	G4 = 391,
	A4 = 440,
	B4 = 493,
	C5 = 523,
	D5 = 587,
	E5 = 659,
	F5 = 698,
	G5 = 783,
	A5 = 880,
	B5 = 987,
	C6 = 1046,
	D6 = 1174,
	E6 = 1328,
	F6 = 1396,
	G6 = 1567,
	A6 = 1760,
	B6 = 1975,
} freq;

class Sound
{
public:
    /** Create a new SoftwarePWM object on any mbed pin
      *
      * @param Pin Pin on mbed to connect PWM device to
     */
    Sound(PinName Ppwm);

    void setPeriod(float seconds);

    void write(float value);

    float read();
    
#ifdef MBED_OPERATORS
    /** A operator shorthand for write()
     */
    Sound& operator= (float value) {
        write(value);
        return *this;
    }

    Sound& operator= (Sound& rhs) {
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
    float frequency;
		float period;
};

#endif
