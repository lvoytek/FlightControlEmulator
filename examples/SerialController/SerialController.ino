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
#include "FlightControlEmulator.h"

FlightControlEmulator controller;

void setup()
{
    Serial.begin(460800);
    Serial.setTimeout(2000);

    while (controller.init() != FLIGHT_SUCCESS)
    {
        Serial.println("Error: init failed");
        delay(1000);
    }
}

void loop()
{
    String out = Serial.readString();
    out.trim();

    if(!out.isEmpty())
    {
        if(out.equals("start"))
        {
            if(controller.start() == FLIGHT_SUCCESS)
                Serial.println("Startup successful");
            else
                Serial.println("Startup failed");
        }
        else if(out.equals("stop"))
        {
            if(controller.stop() == FLIGHT_SUCCESS)
                Serial.println("Shutdown successful");
            else
                Serial.println("Shutdown failed");
        }
        else if(out.equals("idle"))
        {
            if(controller.idle() == FLIGHT_SUCCESS)
                Serial.println("Idle successful");
            else
                Serial.println("Idle failed");
        }
    }
}