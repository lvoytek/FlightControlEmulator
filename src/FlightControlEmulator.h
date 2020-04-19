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

#ifndef FLIGHTCONTROLEMULATOR_H
#define FLIGHTCONTROLEMULATOR_H

#include "PWMHandler.h"

/**
 * @brief The communication protocol for flight control
 * @note Only PWM has been implemented in version 1.0.0
 */
typedef enum
{
    PWM = 0,
    PPM
} FlightProtocol;

/**
 * @brief The set of error codes for flight functions
 */
typedef enum
{
    FLIGHT_SUCCESS = 0,
    FLIGHT_PROTOCOL_FAILURE,
    FLIGHT_MODESWAP_FAILURE,
    FLIGHT_INVALID_INPUT
} FlightControlState;

class FlightControlEmulator
{
protected:
    //The protocol handlers, only allocated and initialized when being used
    PWMHandler * pwm;
    //PPMHandler * ppm;

    //The protocol currently in use
    FlightProtocol activeProtocol;

    //The current percentages for all channels
    float currentValues[6];

public:
    /**
     * @brief Initializes the controller with a given protocol along with the default pins for it
     * 
     * @param protocol The protocol that the system will emulate
     */
    FlightControlEmulator(FlightProtocol protocol);

    /**
     * @brief Initializes the controller with the PWM 6-channel protocol along with its default pins
     */
    FlightControlEmulator() : FlightControlEmulator(PWM) {}

    /**
     * @brief Initializes the active protocol internals
     * 
     * @return
     *     - FLIGHT_SUCCESS the initialization was successful
     *     - FLIGHT_PROTOCOL_FAILURE the initialization failed
     */
    FlightControlState init();

    /**
     * @brief Activates the protocol in the idle, ready for takeoff state
     * 
     * @return
     *     - FLIGHT_SUCCESS the activation was successful
     *     - FLIGHT_PROTOCOL_FAILURE the activation failed
     */
    FlightControlState start();

    /**
     * @brief Deactivates the protocol
     * 
     * @return
     *     - FLIGHT_SUCCESS the deactivation was successful
     *     - FLIGHT_PROTOCOL_FAILURE the deactivation failed
     */
    FlightControlState stop();

    /**
     * @brief Tells the aircraft to idle using active protocol, does not change
     * state of the auxiliary channels
     * 
     * @return
     *     - FLIGHT_SUCCESS the mode change was successful
     *     - FLIGHT_MODESWAP_FAILURE the change to idle failed
     */
    FlightControlState idle();

    /**
     * @brief Sets the throttle level
     * 
     * @param throttleLevel The percentage to set the throttle to
     * 
     * @return
     *     - FLIGHT_SUCCESS the throttle change was successful
     *     - FLIGHT_MODESWAP_FAILURE the throttle set failed as the controller is not initialized
     *     - FLIGHT_INVALID_INPUT a bad percentage value was entered, throttle set failed
     *     - FLIGHT_PROTOCOL_FAILURE The protocol ran into an error
     */
    FlightControlState setThrottle(float throttleLevel);

    /**
     * @brief Sets the elevator direction for planes / upward acceleration for drones
     * 
     * @param elevatorDir The direction to rotate the elevator / upward acceleration multiplier from -1 to 1
     * 
     * @return
     *     - FLIGHT_SUCCESS the elevator change was successful
     *     - FLIGHT_MODESWAP_FAILURE the elevator set failed as the controller is not initialized
     *     - FLIGHT_INVALID_INPUT a bad scale value was entered, elevator set failed
     *     - FLIGHT_PROTOCOL_FAILURE The protocol ran into an error
     */
    FlightControlState pitch(float elevatorDir);

    /**
     * @brief Sets the aileron direction for planes / tilt direction for drones
     * 
     * @param aileronDir The direction to rotate the aileron / clockwise from behind tilt from -1 to 1
     * 
     * @return
     *     - FLIGHT_SUCCESS the aileron change was successful
     *     - FLIGHT_MODESWAP_FAILURE the aileron set failed as the controller is not initialized
     *     - FLIGHT_INVALID_INPUT a bad scale value was entered, aileron set failed
     *     - FLIGHT_PROTOCOL_FAILURE The protocol ran into an error
     */
    FlightControlState roll(float aileronDir);

    /**
     * @brief Sets the rudder direction for planes / clockwise from above rotation scale for drones
     * 
     * @param rudderDir The direction to rotate the rudder / clockwise acceleration multiplier from -1 to 1
     * 
     * @return
     *     - FLIGHT_SUCCESS the rudder change was successful
     *     - FLIGHT_MODESWAP_FAILURE the rudder set failed as the controller is not initialized
     *     - FLIGHT_INVALID_INPUT a bad scale value was entered, rudder set failed
     *     - FLIGHT_PROTOCOL_FAILURE The protocol ran into an error
     */
    FlightControlState yaw(float rudderDir);

    /**
     * @brief Sets pitch, yaw, and roll to center, zero value
     * 
     * 
     * @return
     *     - FLIGHT_SUCCESS the control direction changes were successful
     *     - FLIGHT_MODESWAP_FAILURE the centering failed as the controller is not initialized
     *     - FLIGHT_PROTOCOL_FAILURE The protocol ran into an error
     */
    FlightControlState resetControl();

};


#endif