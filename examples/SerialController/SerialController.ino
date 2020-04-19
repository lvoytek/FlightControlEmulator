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

float getSerialVal(String input)
{
	int spaceLoc = input.lastIndexOf(" ");
	if(spaceLoc > 1)
	{
		String substr = input.substring(spaceLoc + 1);
		return(substr.toFloat());
	}
	else
		return -100;
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
		else if(out.equals("reset"))
		{
			if(controller.resetControl() == FLIGHT_SUCCESS)
				Serial.println("Control reset successful");
			else
				Serial.println("Control reset failed");
		}
		else
		{
			if(out.startsWith("throttle"))
			{
				if(controller.setThrottle(getSerialVal(out)) == FLIGHT_SUCCESS)
					Serial.println("Throttle set successful");
				else
					Serial.println("Throttle set failed");
			}
			else if(out.startsWith("pitch"))
			{
				if(controller.pitch(getSerialVal(out)) == FLIGHT_SUCCESS)
					Serial.println("Pitch successful");
				else
					Serial.println("Pitch failed");
			}
			else if(out.startsWith("roll"))
			{
				if(controller.roll(getSerialVal(out)) == FLIGHT_SUCCESS)
					Serial.println("Roll successful");
				else
					Serial.println("Roll failed");
			}
			else if(out.startsWith("yaw"))
			{
				if(controller.yaw(getSerialVal(out)) == FLIGHT_SUCCESS)
					Serial.println("Yaw successful");
				else
					Serial.println("Yaw failed");
			}
			else if(out.equals("aux1 on"))
			{
				if(controller.activateAUX1() == FLIGHT_SUCCESS)
					Serial.println("Aux1 on successful");
				else
					Serial.println("Aux1 on failed");
			}
			else if(out.equals("aux2 on"))
			{
				if(controller.activateAUX2() == FLIGHT_SUCCESS)
					Serial.println("Aux2 on successful");
				else
					Serial.println("Aux2 on failed");
			}
			else if(out.equals("aux1 off"))
            {
                if(controller.deactivateAUX1() == FLIGHT_SUCCESS)
                    Serial.println("Aux1 off successful");
                else
                    Serial.println("Aux1 off failed");
            }
			else if(out.equals("aux2 off"))
            {
                if(controller.deactivateAUX2() == FLIGHT_SUCCESS)
                    Serial.println("Aux2 off successful");
                else
                    Serial.println("Aux2 off failed");
            }
		}
	}
}