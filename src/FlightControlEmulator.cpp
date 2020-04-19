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

#include "FlightControlEmulator.h"
FlightControlEmulator::FlightControlEmulator(FlightProtocol protocol)
{
    this->activeProtocol = protocol;

    switch(protocol)
    {
        case PPM:
        case PWM:
        default:
            this->pwm = new PWMHandler();
    }

    for(int i = 0; i < 6; i++)
        this->currentValues[i] = 0;
}

FlightControlState FlightControlEmulator::init()
{
    if(this->activeProtocol == PWM)
    {
        if(this->pwm->init() == PWM_SUCCESS)
            return FLIGHT_SUCCESS;
    }

    return FLIGHT_PROTOCOL_FAILURE;
}

FlightControlState FlightControlEmulator::start()
{
    if(this->activeProtocol == PWM)
    {
        if(this->idle() == FLIGHT_SUCCESS && this->pwm->start() == PWM_SUCCESS)
            return FLIGHT_SUCCESS;
    }

    return FLIGHT_PROTOCOL_FAILURE;
}

FlightControlState FlightControlEmulator::stop()
{
    if(this->activeProtocol == PWM)
    {
        if(this->pwm->stop() == PWM_SUCCESS)
            return FLIGHT_SUCCESS;
    }

    return FLIGHT_PROTOCOL_FAILURE;
}

FlightControlState FlightControlEmulator::idle()
{
    if(this->activeProtocol == PWM)
    {
        if(this->pwm->setChannelOutputAllWithTypes(50, 50, 0, 50, this->currentValues[4], this->currentValues[5]) == PWM_SUCCESS)
        {
            this->currentValues[0] = 50;
            this->currentValues[1] = 50;
            this->currentValues[2] = 0;
            this->currentValues[3] = 50;
            return FLIGHT_SUCCESS;
        }
    }

    return FLIGHT_MODESWAP_FAILURE;
}

FlightControlState FlightControlEmulator::setThrottle(float throttleLevel)
{
    if(this->activeProtocol == PWM)
    {
        if(!this->pwm->isInitialized())
            return FLIGHT_MODESWAP_FAILURE;

        if(throttleLevel < 0 || throttleLevel > 100)
            return FLIGHT_INVALID_INPUT;

        if(this->pwm->setChannelOutput(PWM_CHANNEL_THROTTLE, throttleLevel) != PWM_SUCCESS)
            return FLIGHT_PROTOCOL_FAILURE;
    }

    return FLIGHT_SUCCESS;
}

FlightControlState FlightControlEmulator::pitch(float elevatorDir)
{
    if(this->activeProtocol == PWM)
    {
        if(!this->pwm->isInitialized())
            return FLIGHT_MODESWAP_FAILURE;

        if(elevatorDir < -1 || elevatorDir > 1)
            return FLIGHT_INVALID_INPUT;

        if(this->pwm->setChannelOutput(PWM_CHANNEL_ELEVATOR, (elevatorDir + 1) * 50) != PWM_SUCCESS)
            return FLIGHT_PROTOCOL_FAILURE;
    }

    return FLIGHT_SUCCESS;
}

FlightControlState FlightControlEmulator::roll(float aileronDir)
{
    if(this->activeProtocol == PWM)
    {
        if(!this->pwm->isInitialized())
            return FLIGHT_MODESWAP_FAILURE;

        if(aileronDir < -1 || aileronDir > 1)
            return FLIGHT_INVALID_INPUT;

        if(this->pwm->setChannelOutput(PWM_CHANNEL_AILERON, (aileronDir + 1) * 50) != PWM_SUCCESS)
            return FLIGHT_PROTOCOL_FAILURE;
    }

    return FLIGHT_SUCCESS;
}

FlightControlState FlightControlEmulator::yaw(float rudderDir)
{
    if(this->activeProtocol == PWM)
    {
        if(!this->pwm->isInitialized())
            return FLIGHT_MODESWAP_FAILURE;

        if(rudderDir < -1 || rudderDir > 1)
            return FLIGHT_INVALID_INPUT;

        if(this->pwm->setChannelOutput(PWM_CHANNEL_RUDDER, (rudderDir + 1) * 50) != PWM_SUCCESS)
            return FLIGHT_PROTOCOL_FAILURE;
    }

    return FLIGHT_SUCCESS;
}

FlightControlState FlightControlEmulator::resetControl()
{
    if(this->activeProtocol == PWM)
    {
        if(!this->pwm->isInitialized())
            return FLIGHT_MODESWAP_FAILURE;

        if(this->pwm->setChannelOutput(PWM_CHANNEL_ELEVATOR, 50) != PWM_SUCCESS)
            return FLIGHT_PROTOCOL_FAILURE;

        if(this->pwm->setChannelOutput(PWM_CHANNEL_AILERON, 50) != PWM_SUCCESS)
            return FLIGHT_PROTOCOL_FAILURE;

        if(this->pwm->setChannelOutput(PWM_CHANNEL_RUDDER, 50) != PWM_SUCCESS)
            return FLIGHT_PROTOCOL_FAILURE;
    }

    return FLIGHT_SUCCESS;
}