#pragma once

#ifndef _SERVOCONTROL_H_
#define _SERVOCONTROL_H_

#include <Arduino.h>
#include <Servo.h>

class ServoMotor : public Servo
{
public:
    uint8_t pin;
    int lastPos;

    ServoMotor(uint8_t _pin)
    {
        pin = _pin;
        lastPos = 0;
    }

    void init()
    {
        this->attach(pin);
        this->write(lastPos);
    }
};

ServoMotor frontServo(5);
ServoMotor leftServo(4);
ServoMotor rightServo(2);

void initServos()
{
    frontServo.init();
    leftServo.init();
    rightServo.init();
}

void moveServo(ServoMotor *servoMotor, int pos)
{
    if (servoMotor->lastPos > pos)
    {
        for (int i = servoMotor->lastPos; i > pos; i--)
        {
            servoMotor->write(i);
            delay(10);
        }
    }
    else
    {
        for (int i = servoMotor->lastPos; i < pos; i++)
        {
            servoMotor->write(i);
            delay(10);
        }
    }
    servoMotor->lastPos = pos;
}

#endif //_SERVOCONTROL_H_