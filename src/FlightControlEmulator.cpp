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
        if(this->pwm->setChannelOutputAllWithTypes(50, 50, 0, 50, 0, 0) == PWM_SUCCESS)
            return FLIGHT_SUCCESS;
    }

    return FLIGHT_MODESWAP_FAILURE;
}
