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

#include "PWMHandler.h"

PWMHandler::PWMHandler()
{
	this->pwmUnit = MCPWM_UNIT_0;
	this->channelPins[0] = PIN_12;
	this->channelPins[1] = PIN_27;
	this->channelPins[2] = PIN_33;
	this->channelPins[3] = PIN_15;
	this->channelPins[4] = PIN_32;
	this->channelPins[5] = PIN_14;

	this->mcpwmChannelMap[0] = MCPWM0A;
	this->mcpwmChannelMap[1] = MCPWM0B;
	this->mcpwmChannelMap[2] = MCPWM1A;
	this->mcpwmChannelMap[3] = MCPWM1B;
	this->mcpwmChannelMap[4] = MCPWM2A;
	this->mcpwmChannelMap[5] = MCPWM2B;

	for(int i = 0; i < 3; i++)
	{
		this->configurationData[i].cmpr_a = 5.0;
		this->configurationData[i].cmpr_b = 5.0;
		this->configurationData[i].duty_mode = MCPWM_DUTY_MODE_0;
		this->configurationData[i].counter_mode = MCPWM_UP_COUNTER;
	}
}

PWMHandler::PWMHandler(mcpwm_unit_t pwmUnit, int channel1, int channel2, int channel3, int channel4, int channel5, int channel6)
{
	if(pwmUnit >= MCPWM_UNIT_MAX)
		this->pwmUnit = MCPWM_UNIT_0;
	else
		this->pwmUnit = pwmUnit;

	this->channelPins[0] = channel1;
	this->channelPins[1] = channel2;
	this->channelPins[2] = channel3;
	this->channelPins[3] = channel4;
	this->channelPins[4] = channel5;
	this->channelPins[5] = channel6;

	this->mcpwmChannelMap[0] = MCPWM0A;
	this->mcpwmChannelMap[1] = MCPWM0B;
	this->mcpwmChannelMap[2] = MCPWM1A;
	this->mcpwmChannelMap[3] = MCPWM1B;
	this->mcpwmChannelMap[4] = MCPWM2A;
	this->mcpwmChannelMap[5] = MCPWM2B;
}

pwm_state PWMHandler::init()
{
	//Initialize pins
	for(int i = 0; i < 6; i++)
	{
		if(mcpwm_gpio_init(this->pwmUnit, this->mcpwmChannelMap[i], this->channelPins[i]) != ESP_OK)
			return PWM_FAILURE;
	}

	//Initialize timer configs
	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_init(this->pwmUnit, (mcpwm_timer_t) i, this->configurationData + i) != ESP_OK)
			return PWM_FAILURE;
	}

	//Set default frequency
	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_set_frequency(this->pwmUnit, (mcpwm_timer_t) i, PWM_DEFAULT_APPROX_FREQUENCY_HZ) != ESP_OK)
			return PWM_FAILURE;
	}

	return PWM_SUCCESS;
}

pwm_state PWMHandler::start()
{
	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_start(this->pwmUnit, (mcpwm_timer_t) i) != ESP_OK)
			return PWM_FAILURE;
	}

	return PWM_SUCCESS;
}

pwm_state PWMHandler::stop()
{
	for(int i = 0; i < 3; i++)
	{
		if(mcpwm_stop(this->pwmUnit, (mcpwm_timer_t) i) != ESP_OK)
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
	if(mcpwm_set_duty(this->pwmUnit, (mcpwm_timer_t) (channel/2), (mcpwm_operator_t) (channel%2), dutyPercentage) != ESP_OK)
		return PWM_FAILURE;

	return PWM_SUCCESS;
}