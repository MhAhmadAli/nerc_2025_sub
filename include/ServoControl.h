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
        lastPos = 90;
    }

    void init(int pos = -1)
    {
        this->attach(pin);
        this->write(pos != -1 ? pos : lastPos);
    }
};

ServoMotor frontServo(5);
ServoMotor leftServo(4);
ServoMotor rightServo(2);
ServoMotor baseServo(3);

void initServos()
{
    frontServo.init();
    leftServo.init();
    rightServo.init();
    baseServo.init(10);
}

void moveServo(ServoMotor *servoMotor, int pos)
{
    noInterrupts();
    if (servoMotor->lastPos > pos)
    {
        for (int i = servoMotor->lastPos; i > pos; i--)
        {
            servoMotor->write(i);
            delay(30);
        }
    }
    else
    {
        for (int i = servoMotor->lastPos; i < pos; i++)
        {
            servoMotor->write(i);
            delay(30);
        }
    }
    servoMotor->lastPos = pos;
    interrupts();
}

#endif //_SERVOCONTROL_H_