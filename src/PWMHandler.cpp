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
#include <Arduino.h>
#include "PWMHandler.h"
PWMHandler::PWMHandler(mcpwm_unit_t pwmUnit1, mcpwm_unit_t pwmUnit2, int channel1, int channel2, int channel3, int channel4, int channel5, int channel6)
{
	if(pwmUnit1 >= MCPWM_UNIT_MAX)
		this->pwmUnits[0] = MCPWM_UNIT_0;
	else
		this->pwmUnits[0] = pwmUnit1;

	if(pwmUnit2 >= MCPWM_UNIT_MAX)
		this->pwmUnits[1] = MCPWM_UNIT_1;
	else
		this->pwmUnits[1] = pwmUnit2;

	this->channelPins[0] = channel1;
	this->channelPins[1] = channel2;
	this->channelPins[2] = channel3;
	this->channelPins[3] = channel4;
	this->channelPins[4] = channel5;
	this->channelPins[5] = channel6;

	this->mcpwmChannelMap[0] = MCPWM0A;
	this->mcpwmChannelMap[1] = MCPWM1A;
	this->mcpwmChannelMap[2] = MCPWM2A;
	this->mcpwmChannelMap[3] = MCPWM0A;
	this->mcpwmChannelMap[4] = MCPWM1A;
	this->mcpwmChannelMap[5] = MCPWM2A;

	this->unitChannelMap[0] = this->pwmUnits[0];
	this->unitChannelMap[1] = this->pwmUnits[0];
	this->unitChannelMap[2] = this->pwmUnits[0];
	this->unitChannelMap[3] = this->pwmUnits[1];
	this->unitChannelMap[4] = this->pwmUnits[1];
	this->unitChannelMap[5] = this->pwmUnits[1];

	this->channelMinimums[PWM_CHANNEL_AILERON -1] = PWM_DUTY_AILERON_MINIMUM;
	this->channelMinimums[PWM_CHANNEL_ELEVATOR -1] = PWM_DUTY_ELEVATOR_MINIMUM;
	this->channelMinimums[PWM_CHANNEL_THROTTLE -1] = PWM_DUTY_THROTTLE_MINIMUM;
	this->channelMinimums[PWM_CHANNEL_RUDDER -1] = PWM_DUTY_RUDDER_MINIMUM;
	this->channelMinimums[PWM_CHANNEL_AUX_A -1] = PWM_DUTY_AUX_MINIMUM;
	this->channelMinimums[PWM_CHANNEL_AUX_B -1] = PWM_DUTY_AUX_MINIMUM;

	this->channelMaximums[PWM_CHANNEL_AILERON -1] = PWM_DUTY_AILERON_MAXIMUM;
	this->channelMaximums[PWM_CHANNEL_ELEVATOR -1] = PWM_DUTY_ELEVATOR_MAXIMUM;
	this->channelMaximums[PWM_CHANNEL_THROTTLE -1] = PWM_DUTY_THROTTLE_MAXIMUM;
	this->channelMaximums[PWM_CHANNEL_RUDDER -1] = PWM_DUTY_RUDDER_MAXIMUM;
	this->channelMaximums[PWM_CHANNEL_AUX_A -1] = PWM_DUTY_AUX_MAXIMUM;
	this->channelMaximums[PWM_CHANNEL_AUX_B -1] = PWM_DUTY_AUX_MAXIMUM;

	for(int i = 0; i < 6; i++)
	{
		this->configurationData[i].frequency = PWM_DEFAULT_APPROX_FREQUENCY_HZ;
		this->configurationData[i].cmpr_a = 0;
		this->configurationData[i].cmpr_b = 0;
		this->configurationData[i].duty_mode = MCPWM_DUTY_MODE_0;
		this->configurationData[i].counter_mode = MCPWM_UP_COUNTER;
	}

	for(int i = 0; i < 6; i++)
		this->currentDutys[i] = 0.0;
}

pwm_state PWMHandler::init()
{
	mcpwm_gpio_init(this->pwmUnits[0], MCPWM_SYNC_0, PIN_A0);
	mcpwm_gpio_init(this->pwmUnits[1], MCPWM_SYNC_0, PIN_A0);

	//Initialize pins
	for(int i = 0; i < 6; i++)
	{
		if(mcpwm_gpio_init(this->unitChannelMap[i], this->mcpwmChannelMap[i], this->channelPins[i]) != ESP_OK)
			return PWM_FAILURE;
	}

	//Initialize timer configs
	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_init(this->pwmUnits[0], (mcpwm_timer_t) i, &this->configurationData[i]) != ESP_OK)
			return PWM_FAILURE;
	}

	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_init(this->pwmUnits[1], (mcpwm_timer_t) i, &this->configurationData[i + 3]) != ESP_OK)
			return PWM_FAILURE;
	}

	//Set default frequency
	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_set_frequency(this->pwmUnits[0], (mcpwm_timer_t) i, PWM_DEFAULT_APPROX_FREQUENCY_HZ) != ESP_OK)
			return PWM_FAILURE;
	}

	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_set_frequency(this->pwmUnits[1], (mcpwm_timer_t) i, PWM_DEFAULT_APPROX_FREQUENCY_HZ) != ESP_OK)
			return PWM_FAILURE;
	}

	this->pwmFrequency = PWM_DEFAULT_APPROX_FREQUENCY_HZ;

	return PWM_SUCCESS;
}

pwm_state PWMHandler::start()
{
	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_start(this->pwmUnits[0], (mcpwm_timer_t) i) != ESP_OK)
			return PWM_FAILURE;
	}

	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_start(this->pwmUnits[1], (mcpwm_timer_t) i) != ESP_OK)
			return PWM_FAILURE;
	}

	return PWM_SUCCESS;
}

pwm_state PWMHandler::stop()
{
	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_set_duty(this->pwmUnits[0], (mcpwm_timer_t) i, MCPWM_OPR_A, 0) != ESP_OK || mcpwm_stop(this->pwmUnits[0], (mcpwm_timer_t) i) != ESP_OK)
			return PWM_FAILURE;
	}

	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_set_duty(this->pwmUnits[1], (mcpwm_timer_t) i, MCPWM_OPR_A, 0) != ESP_OK || mcpwm_stop(this->pwmUnits[1], (mcpwm_timer_t) i) != ESP_OK)
			return PWM_FAILURE;
	}



	return PWM_SUCCESS;
}

pwm_state PWMHandler::setDutyAll(float channel1, float channel2, float channel3, float channel4, float channel5, float channel6)
{
	if(this->setDuty(1, channel1) == PWM_FAILURE) return PWM_FAILURE;
	if(this->setDuty(2, channel2) == PWM_FAILURE) return PWM_FAILURE;
	if(this->setDuty(3, channel3) == PWM_FAILURE) return PWM_FAILURE;
	if(this->setDuty(4, channel4) == PWM_FAILURE) return PWM_FAILURE;
	if(this->setDuty(5, channel5) == PWM_FAILURE) return PWM_FAILURE;
	if(this->setDuty(6, channel6) == PWM_FAILURE) return PWM_FAILURE;

	return PWM_SUCCESS;
}

pwm_state PWMHandler::setDuty(int channel, float dutyPercentage)
{
	if(channel < 1 || channel > 6)
		return PWM_INVALID_CHANNEL;

	channel --;
	this->currentDutys[channel] = dutyPercentage;
	
	for(int i = channel; i < 6; i++)
	{
		float delayPercent = 0;

		for(int j = 0; j < i; j++)
			delayPercent += this->currentDutys[j];

		if(mcpwm_sync_enable(this->unitChannelMap[i], (mcpwm_timer_t) (i%3), MCPWM_SELECT_SYNC0, int(1000-delayPercent*10)) != ESP_OK)
			return PWM_FAILURE;

		if(mcpwm_set_duty(this->unitChannelMap[i], (mcpwm_timer_t) (i%3), MCPWM_OPR_A, this->currentDutys[i]) != ESP_OK)
			return PWM_FAILURE;
	}

	return PWM_SUCCESS;
}

pwm_state PWMHandler::setChannelOutput(int channel, float percentage)
{
	if(percentage < 0 || percentage > 100)
		return PWM_OUT_OF_RC_Range;

	return setDuty(channel, (this->channelMaximums[channel - 1] - this->channelMinimums[channel - 1]) * .01 * percentage + this->channelMinimums[channel - 1]);
}

pwm_state PWMHandler::setChannelOutputAll(float channel1, float channel2, float channel3, float channel4, float channel5, float channel6)
{
	pwm_state returnValue;
	if((returnValue = this->setChannelOutput(1, channel1)) != PWM_SUCCESS) return returnValue;
	if((returnValue = this->setChannelOutput(2, channel2)) != PWM_SUCCESS) return returnValue;
	if((returnValue = this->setChannelOutput(3, channel3)) != PWM_SUCCESS) return returnValue;
	if((returnValue = this->setChannelOutput(4, channel4)) != PWM_SUCCESS) return returnValue;
	if((returnValue = this->setChannelOutput(5, channel5)) != PWM_SUCCESS) return returnValue;
	returnValue = this->setChannelOutput(6, channel6);
	
	return returnValue;
}

pwm_state PWMHandler::setChannelOutputAllWithTypes(float aileron, float throttle, float elevator, float rudder, float aux1, float aux2)
{
	pwm_state returnValue;
	if((returnValue = this->setChannelOutput(PWM_CHANNEL_AILERON, aileron)) != PWM_SUCCESS) return returnValue;
	if((returnValue = this->setChannelOutput(PWM_CHANNEL_THROTTLE, throttle)) != PWM_SUCCESS) return returnValue;
	if((returnValue = this->setChannelOutput(PWM_CHANNEL_ELEVATOR, elevator)) != PWM_SUCCESS) return returnValue;
	if((returnValue = this->setChannelOutput(PWM_CHANNEL_RUDDER, rudder)) != PWM_SUCCESS) return returnValue;
	if((returnValue = this->setChannelOutput(PWM_CHANNEL_AUX_A, aux1)) != PWM_SUCCESS) return returnValue;
	returnValue = this->setChannelOutput(PWM_CHANNEL_AUX_B, aux2);
	
	return returnValue;
}