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

/**
 * @brief Enumeration of MCPWM capable pins on the Adafruit ESP32 Feather
 */
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


/**
 * @brief PWM function return values
 */
typedef enum
{
	PWM_SUCCESS = 0,
	PWM_FAILURE,
	PWM_INVALID_CHANNEL,
	PWM_OUT_OF_RC_Range
} pwm_state;


class PWMHandler
{
protected:
	//Map of channels to hardware pins
	int channelPins[6];

	//Map of MCPWXA/MCPWXB macros to channels
	mcpwm_io_signals_t mcpwmChannelMap[6];

	//Minimum PWM duty cycle value for each channel when it is online for remote controlling
	float channelMinimums[6];

	//Maximum PWM duty cycle value for each channel when it is online for remote controlling
	float channelMaximums[6];

	//The pwm control unit being used
	mcpwm_unit_t pwmUnit;

	//The control unit timer being used

	//MCPWM configuration settings for each timer
	mcpwm_config_t configurationData[3];

	//The base pwm frequency
	int pwmFrequency;

public:
	/**
	 * @brief Set specified PWM pins using a given MCPWM unit 
	 * 
	 * @param pwmUnit The MCPWM unit to use for timing
	 * @param channel1 PWM Channel 1 output GPIO pin
	 * @param channel2 PWM Channel 2 output GPIO pin
	 * @param channel3 PWM Channel 3 output GPIO pin
	 * @param channel4 PWM Channel 4 output GPIO pin
	 * @param channel5 PWM Channel 5 output GPIO pin
	 * @param channel6 PWM Channel 6 output GPIO pin
	 */
	PWMHandler(mcpwm_unit_t pwmUnit, int channel1, int channel2, int channel3, int channel4, int channel5, int channel6);
	
	/**
	 * @brief Set default Feather PWM pins on MCPWM unit 0
	 */
	PWMHandler() : PWMHandler(MCPWM_UNIT_0, PIN_12, PIN_27, PIN_33, PIN_15, PIN_32, PIN_14) {}

	/**
	 * @brief Initialize the mcpwm unit and prepare for gpio output use
	 * 
	 * @return
	 *     - PWM_SUCCESS Initialization successful
	 *     - PWM_FAILURE MCPWMn failure
	 */
	pwm_state init();

	/**
	 * @brief Activate all PWM outputs in current configuration
	 * 
	 * @return
	 *     - PWM_SUCCESS Successful start
	 *     - PWM_FAILURE Activation failed
	 */
	pwm_state start();

	/**
	 * @brief Deactivate all PWM outputs in current configuration
	 * 
	 * @return
	 *     - PWM_SUCCESS Successful stop
	 *     - PWM_FAILURE Deactivation failed
	 */
	pwm_state stop();

	/**
	 * @brief Set the positive PWM duty cycle percentage of all channels
	 * 
	 * @param channel1 The PWM Positive Duty % for the channel 1 pin
	 * @param channel2 The PWM Positive Duty % for the channel 2 pin
	 * @param channel3 The PWM Positive Duty % for the channel 3 pin
	 * @param channel4 The PWM Positive Duty % for the channel 4 pin
	 * @param channel5 The PWM Positive Duty % for the channel 5 pin
	 * @param channel6 The PWM Positive Duty % for the channel 6 pin
	 * 
	 * @return
	 *     - PWM_SUCCESS Successful duty change
	 *     - PWM_FAILURE Duty change failed
	 */
	pwm_state setDutyAll(float channel1, float channel2, float channel3, float channel4, float channel5, float channel6);

	/**
	 * @brief Set the positive PWM duty cycle percentage for the pin on the given channel
	 * 
	 * @param channel The channel to change the duty cycle of
	 * @param dutyPercentage The positive duty cycle percentage to change to
	 * 
	 * @return
	 *     - PWM_SUCCESS Successful duty change
	 *     - PWM_FAILURE Duty change failed
	 *     - PWM_INVALID_CHANNEL The given channel number is not 1-6, no change
	 */
	pwm_state setDuty(int channel, float dutyPercentage);


	/**
	 * @brief Set the PWM duty cycle percentage of a channel to a percentage that defines where it should be in the
	 * area of acceptable duty cycles for the given RC channel
	 * 
	 * @param channel The channel to change the RC value of
	 * @param percentage The scalar location of where it is between minimum and maximum acceptable PWM duty values
	 * 
	 * @return
	 *     - PWM_SUCCESS Successful duty change
	 *     - PWM_FAILURE Duty change failed
	 *     - PWM_INVALID_CHANNEL The given channel number is not 1-6, no change
	 *     - PWM_OUT_OF_RC_RANGE The percentage value places duty cycle out of RC range, no change
	 */
	pwm_state setChannelOutput(int channel, float percentage);


	/**
	 * @brief Set the PWM duty cycle percentage of all channel to percentages that define where they should be in the
	 * area of acceptable duty cycles for each RC channel
	 * 
	 * @param channel1 The RC output percentage for the channel 1 pin
	 * @param channel2 The RC output percentage for the channel 2 pin
	 * @param channel3 The RC output percentage for the channel 3 pin
	 * @param channel4 The RC output percentage for the channel 4 pin
	 * @param channel5 The RC output percentage for the channel 5 pin
	 * @param channel6 The RC output percentage for the channel 6 pin
	 * 
	 * @return
	 *     - PWM_SUCCESS Successful duty change
	 *     - PWM_FAILURE Duty change failed
	 *     - PWM_OUT_OF_RC_RANGE The percentage value for at least 1 channel places duty cycle out of RC range, no change
	 */
	pwm_state setChannelOutputAll(float channel1, float channel2, float channel3, float channel4, float channel5, float channel6);
};

#endif