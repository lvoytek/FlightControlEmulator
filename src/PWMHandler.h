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

//Macros for PWM configurations for 6-channel mode based on experimental data
#define PWM_DEFAULT_PERIOD_S .018302

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

#endif