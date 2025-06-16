#pragma once

#ifndef _COLORSENSORS_H_
#define _COLORSENSORS_H_

#include <Arduino.h>

#define COLOR_CENT_SENSOR 10 // center color sensor pin
#define COLOR_LEFT_SENSOR 9. // left color sensor pin

#define COLOR_RED_THRESHOLD 150 // threshold for matching red color value

// Initialize color sensor pins
void initColorSensors()
{
    pinMode(COLOR_CENT_SENSOR, INPUT);
    pinMode(COLOR_LEFT_SENSOR, INPUT);
}

//The ```isRed``` function determines whether a sensor detects a red colour
//by measuring the pulse duration using ```pulseIn``` and comparing the result
//against a predefined threshold ```(COLOR_RED_THRESHOLD)```. It returns ```true```
//if the value is below the threshold, indicating ```red```, and ```false``` otherwise.
bool isRed(uint8_t sensor)
{
    int val = pulseIn(sensor, digitalRead(sensor) == HIGH ? LOW : HIGH);
    // Serial.println(val);
    return val < COLOR_RED_THRESHOLD ? true : false;
}

#endif //_COLORSENSORS_H_