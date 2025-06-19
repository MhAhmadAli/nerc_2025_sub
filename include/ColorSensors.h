#pragma once

#ifndef _COLORSENSORS_H_
#define _COLORSENSORS_H_

#include <Arduino.h>

#define COLOR_CENT_SENSOR 10 // center color sensor pin
#define COLOR_LEFT_SENSOR 9  // left color sensor pin

// these thresholds are only for red colors
#define COLOR_LEFT_THRESHOLD 150
#define COLOR_CENT_THRESHOLD 600

// Initialize color sensor pins
void initColorSensors()
{
    pinMode(COLOR_CENT_SENSOR, INPUT);
    pinMode(COLOR_LEFT_SENSOR, INPUT);
}

void printColorSensor(uint8_t color_pin)
{
    Serial.println(pulseIn(color_pin, digitalRead(color_pin) == HIGH ? LOW : HIGH));
}

// The ```isRed``` function determines whether a sensor detects a red colour
// by measuring the pulse duration using ```pulseIn``` and comparing the result
// against a predefined threshold ```(COLOR_RED_THRESHOLD)```. It returns ```true```
// if the value is below the threshold, indicating ```red```, and ```false``` otherwise.
bool isRed(uint8_t sensor)
{
    int threshold = sensor == COLOR_LEFT_SENSOR ? COLOR_LEFT_THRESHOLD : COLOR_CENT_THRESHOLD;
    int val = pulseIn(sensor, digitalRead(sensor) == HIGH ? LOW : HIGH);
    // Serial.println(val);
    return val < threshold ? true : false;
}

String checkForRed()
{
    if (isRed(COLOR_CENT_SENSOR))
    {
        return "CENT";
    }
    else if (isRed(COLOR_LEFT_SENSOR))
    {
        return "LEFT";
    }
    else
    {
        return "RGHT";
    }
}

#endif //_COLORSENSORS_H_