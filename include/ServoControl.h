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

// TODO: Remove classes and write simple code as there were memory issues.

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

    void init(int pos1 = 90, int pos2 = 0)
    {
        flapServo1.init(pinServo1, pos1);
        flapServo2.init(pinServo2, pos2);
    }

    void open(int pos1 = 90, int pos2 = 0)
    {
        flapServo1.write(pos1);
        flapServo2.write(pos2);
    }

    void close(int pos1 = 0, int pos2 = 90)
    {
        flapServo1.write(pos1);
        flapServo2.write(pos2);
    }
};

PickupServo frontServo;
PickupServo leftServo;
PickupServo rightServo;
PickupServo baseServo;

FlapServos flapLeftServo(6, 7);
FlapServos flapRghtServo(8, 11);
// FlapServos flapCentServo(13, 12);

Servo flapCentServo1;
Servo flapCentServo2;

void openCentFlap(int pos1 = 90, int pos2 = 0)
{
    flapCentServo1.write(pos1);
    flapCentServo2.write(pos2);
}

void closeCentFlap(int pos1 = 0, int pos2 = 90)
{
    flapCentServo1.write(pos1);
    flapCentServo2.write(pos2);
}

void initServos()
{
    frontServo.init(PIN_SERVO_FRONT);
    leftServo.init(PIN_SERVO_LEFT);
    rightServo.init(PIN_SERVO_RIGHT);
    delay(100); // need this delay so the flaps don't collide with corners
    baseServo.init(PIN_SERVO_BASE, POS_SERVO_BASE_000);

    flapLeftServo.init();
    flapRghtServo.init();
    // flapCentServo.init();

    flapCentServo1.attach(12);
    flapCentServo2.attach(13);

    closeCentFlap();
    flapLeftServo.close();
    flapRghtServo.close();

    frontServo.write(70);
    leftServo.write(70);
    rightServo.write(70);

    openCentFlap();
    flapLeftServo.open();
    flapRghtServo.open();
}

void moveServo(PickupServo &servoMotor, int pos)
{
    if (servoMotor.lastPos > pos)
    {
        for (int i = servoMotor.lastPos; i > pos; i--)
        {
            servoMotor.write(i);

            delay(10);
        }
    }
    else
    {
        for (int i = servoMotor.lastPos; i < pos; i++)
        {
            servoMotor.write(i);
            delay(10);
        }
    }
    servoMotor.lastPos = pos;
}

#endif //_SERVOCONTROL_H_