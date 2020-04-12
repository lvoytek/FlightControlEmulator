/*
* Copyright (c) 2020 Lena Voytek
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef PWMHANDLER_H
#define PWMHANDLER_H

#include <driver/mcpwm.h>

//Macros for PWM configurations for 6-channel mode based on experimental data
#define PWM_DEFAULT_PERIOD_S .018302
#define PWM_DEFAULT_FREQUENCY_HZ 54.6388
#define PWM_DEFAULT_APPROX_FREQUENCY_HZ 55

#define PWM_DUTY_AILERON_MINIMUM 5.54455
#define PWM_DUTY_AILERON_MAXIMUM 10.84444
#define PWM_DUTY_THROTTLE_MINIMUM 5.44339
#define PWM_DUTY_THROTTLE_MAXIMUM 10.87176
#define PWM_DUTY_ELEVATOR_MINIMUM 5.90324
#define PWM_DUTY_ELEVATOR_MAXIMUM 10.86134
#define PWM_DUTY_RUDDER_MINIMUM 5.44188
#define PWM_DUTY_RUDDER_MAXIMUM 10.80428
#define PWM_DUTY_AUX_MINIMUM 5.43988
#define PWM_DUTY_AUX_MAXIMUM 10.87071

#define PWM_CHANNEL_AILERON 1
#define PWM_CHANNEL_THROTTLE 2
#define PWM_CHANNEL_ELEVATOR 3
#define PWM_CHANNEL_RUDDER 4
#define PWM_CHANNEL_AUX_A 5
#define PWM_CHANNEL_AUX_B 6

#define PWM_SUCCESS 0
#define PWM_FAILURE 1

//Enumeration of MCPWM capable pins on the Adafruit ESP32 Feather
typedef enum
{
	PIN_A0 = 26,
	PIN_A1 = 25,
	PIN_A5 = 4,
	PIN_21 = 21,
	PIN_13 = 13,
	PIN_12 = 12,
	PIN_27 = 27,
	PIN_33 = 33,
	PIN_15 = 15,
	PIN_32 = 32,
	PIN_14 = 14
} feather_pwm_capable_pins;

class PWMHandler
{
protected:
	//Map of channels to hardware pins
	int channelPins[6];

	//Map of MCPWXA/MCPWXB macros to channels
	mcpwm_io_signals_t mcpwmChannelMap[6];

	//The pwm control unit being used
	mcpwm_unit_t pwmUnit;

	//The control unit timer being used

	//MCPWM configuration settings for each timer
	mcpwm_config_t configurationData[3];

	//The base pwm frequency
	int pwmFrequency;

public:
	/*
	 * Set default Feather PWM pins on MCPWM unit 0
	*/
	PWMHandler();

	/*
	 * Set PWM pins using a given MCPWM unit 
	 */
	PWMHandler(mcpwm_unit_t pwmUnit, int channel1, int channel2, int channel3, int channel4, int channel5, int channel6);
	
	/*
	 * Initialize the mcpwm unit and prepare for gpio output use
	 * 
	 * returns PWM_SUCCESS on proper initialization and PWM_FAILURE on MCPWMn failure
	 */
	int init();

};

#endif