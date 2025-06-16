#pragma once

#ifndef _SERVOCONTROL_H_
#define _SERVOCONTROL_H_

#include <Arduino.h>
#include <Servo.h>
#include "ColorSensors.h"

#define PIN_SERVO_FRONT 5
#define PIN_SERVO_LEFT 4
#define PIN_SERVO_RIGHT 2
#define PIN_SERVO_BASE 3

#define POS_SERVO_BASE_000 10  // LEFT AT FRONT, CENTER AT RIGHT, RIGHT AT BACK
#define POS_SERVO_BASE_090 70  // LEFT AT LEFT, CENTER AT FRONT, RIGHT AT RIGHT
#define POS_SERVO_BASE_180 135 // LEFT AT BACK, CENTER AT LEFT, RIGHT AT FRONT

enum PickupServoEnum
{
    SERVO_NONE,
    SERVO_CENT,
    SERVO_LEFT,
    SERVO_RGHT,
    SERVO_BASE
};

enum FlapServoEnum
{
    FLAP_NONE,
    FLAP_LEFT,
    FLAP_CENT,
    FLAP_RGHT
};

enum FlapState
{
    FLAP_OPEN,
    FLAP_CLOSE
};

enum DirectionEnum
{
    NONE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class PickupServo : public Servo
{
public:
    uint8_t pin;
    int lastPos;

    PickupServo()
    {
        pin = 0;
        lastPos = 90;
    }

    void init(uint8_t _pin, int pos = -1)
    {
        pin = _pin;
        this->attach(pin);
        this->write(pos != -1 ? pos : lastPos);
    }
};

class FlapServos : public PickupServo
{
    private:
    uint8_t pinServo1;
    uint8_t pinServo2;
public:
    PickupServo flapServo1;
    PickupServo flapServo2;

    FlapServos(uint8_t _pinServo1, uint8_t _pinServo2)
    {
        pinServo1 = _pinServo1;
        pinServo2 = _pinServo2;
    }

    void init() {
        flapServo1.init(pinServo1, 90);
        flapServo2.init(pinServo2, 0);
    }

    void open() {
        flapServo1.write(90);
        flapServo2.write(0);
    }

    void close() {
        flapServo1.write(0);
        flapServo2.write(90);
    }
};

PickupServo frontServo;
PickupServo leftServo;
PickupServo rightServo;
PickupServo baseServo;

FlapServos flapLeftServo(6, 7);

void initServos()
{
    frontServo.init(PIN_SERVO_FRONT);
    leftServo.init(PIN_SERVO_LEFT);
    rightServo.init(PIN_SERVO_RIGHT);
    delay(100); // need this delay so the flaps don't collide with corners
    baseServo.init(PIN_SERVO_BASE, POS_SERVO_BASE_000);

    flapLeftServo.init();
}

void moveServo(PickupServo *servoMotor, int pos)
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

void moveBaseServoToDropDirection(DirectionEnum directionToGo, PickupServoEnum servoToFocus)
{
    if (servoToFocus == PickupServoEnum::SERVO_CENT)
    {
        moveServo(&baseServo, POS_SERVO_BASE_090);
    }
    else if (servoToFocus == PickupServoEnum::SERVO_LEFT)
    {
        if (directionToGo == DirectionEnum::FORWARD)
        {
            moveServo(&baseServo, POS_SERVO_BASE_000);
        }
        else if (directionToGo == DirectionEnum::BACKWARD)
        {
            moveServo(&baseServo, POS_SERVO_BASE_180);
        }
    }
    else if (servoToFocus == PickupServoEnum::SERVO_RGHT)
    {
        if (directionToGo == DirectionEnum::FORWARD)
        {
            moveServo(&baseServo, POS_SERVO_BASE_180);
        }
        else if (directionToGo == DirectionEnum::BACKWARD)
        {
            moveServo(&baseServo, POS_SERVO_BASE_000);
        }
    }
}

void setupForDrop(DirectionEnum directionToGo, bool *shouldDropRed)
{
    if (shouldDropRed)
    {
        if (isRed(COLOR_CENT_SENSOR))
        {
            moveBaseServoToDropDirection(directionToGo, PickupServoEnum::SERVO_CENT);
        }
        else if (isRed(COLOR_LEFT_SENSOR))
        {
            moveBaseServoToDropDirection(directionToGo, PickupServoEnum::SERVO_LEFT);
        }
        else
        {
            moveBaseServoToDropDirection(directionToGo, PickupServoEnum::SERVO_RGHT);
        }
        *shouldDropRed = false;
    }
}

#endif //_SERVOCONTROL_H_