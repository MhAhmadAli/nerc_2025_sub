#include <Arduino.h>
#include <CircularBuffer.hpp>
#include <Wire.h>
#include "ServoControl.h"
#include "ColorSensors.h"

bool shouldDropRed = true;

typedef struct
{
    PickupServoEnum servo;
    int pos;
    FlapServoEnum flap;
    FlapState flapState;
} Cmd;

CircularBuffer<Cmd, 10> queue;

void requestEvent()
{
    Wire.write(checkForRed().c_str());
}

void receiveEvent(int howMany)
{
    String data = "";
    while (Wire.available() > 0) // loop through all but the last
    {
        char chr = Wire.read(); // receive byte as a character
        data += String(chr);
    }

    Serial.println(data);

    if (data.indexOf("DR") > -1)
    {
        if (data.substring(data.indexOf(" ")).indexOf("FT") > -1)
        {
            queue.push({
                servo : PickupServoEnum::SERVO_NONE,
                pos : 0,
                flap : FlapServoEnum::FLAP_NONE,
                flapState : FlapState::FLAP_OPEN
            });
        }
        else if (data.substring(data.indexOf(" ")).indexOf("LT") > -1)
        {
            queue.push({
                servo : PickupServoEnum::SERVO_NONE,
                pos : 0,
                flap : FlapServoEnum::FLAP_NONE,
                flapState : FlapState::FLAP_OPEN
            });
        }
        else if (data.substring(data.indexOf(" ")).indexOf("RT") > -1)
        {
            queue.push({
                servo : PickupServoEnum::SERVO_NONE,
                pos : 0,
                flap : FlapServoEnum::FLAP_NONE,
                flapState : FlapState::FLAP_OPEN
            });
        }
        else if (data.substring(data.indexOf(" ")).indexOf("BS") > -1)
        {
            queue.push({
                servo : PickupServoEnum::SERVO_NONE,
                pos : 0,
                flap : FlapServoEnum::FLAP_NONE,
                flapState : FlapState::FLAP_OPEN
            });
        }
    }
    else if (data.indexOf("FL") > -1)
    {
        FlapServoEnum flapServo = FlapServoEnum::FLAP_NONE;
        FlapState flapState = FlapState::FLAP_OPEN;

        if (data.indexOf("FT") > -1)
        {
            flapServo = FlapServoEnum::FLAP_CENT;
        }
        else if (data.indexOf("LT") > -1)
        {
            flapServo = FlapServoEnum::FLAP_LEFT;
            Serial.println("LEFT FLAP");
        }
        else if (data.indexOf("RT") > -1)
        {
            flapServo = FlapServoEnum::FLAP_RGHT;
        }

        if (data.indexOf("CLOSE") > -1)
        {
            flapState = FlapState::FLAP_CLOSE;
            Serial.println("FLAP CLOSE");
        }

        queue.push({
            servo : PickupServoEnum::SERVO_NONE,
            pos : 0,
            flap : flapServo,
            flapState : flapState
        });
    }
    else if (data.indexOf("FT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        queue.push({
            servo : PickupServoEnum::SERVO_CENT,
            pos : angle,
            flap : FlapServoEnum::FLAP_NONE,
            flapState : FlapState::FLAP_OPEN
        });
    }
    else if (data.indexOf("LT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        queue.push({
            servo : PickupServoEnum::SERVO_LEFT,
            pos : angle,
            flap : FlapServoEnum::FLAP_NONE,
            flapState : FlapState::FLAP_OPEN
        });
    }
    else if (data.indexOf("RT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        queue.push({
            servo : PickupServoEnum::SERVO_RGHT,
            pos : angle,
            flap : FlapServoEnum::FLAP_NONE,
            flapState : FlapState::FLAP_OPEN
        });
    }
    else if (data.indexOf("BS") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        queue.push({
            servo : PickupServoEnum::SERVO_BASE,
            pos : angle,
            flap : FlapServoEnum::FLAP_NONE,
            flapState : FlapState::FLAP_OPEN
        });
    }
}

void setup()
{
    Wire.begin(4); // join i2c bus with address #4
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);  
    Serial.begin(9600);

    initServos();
}

void loop()
{
    while (!queue.isEmpty())
    {
        Cmd cmd = queue.pop();

        if (cmd.servo == PickupServoEnum::SERVO_CENT)
        {
            moveServo(frontServo, cmd.pos);
        }
        else if (cmd.servo == PickupServoEnum::SERVO_LEFT)
        {
            moveServo(leftServo, cmd.pos);
        }
        else if (cmd.servo == PickupServoEnum::SERVO_RGHT)
        {
            moveServo(rightServo, cmd.pos);
        }
        else if (cmd.servo == PickupServoEnum::SERVO_BASE)
        {
            moveServo(baseServo, cmd.pos);
        }
        else if (cmd.flap != FlapServoEnum::FLAP_NONE)
        {
            if (cmd.flap == FlapServoEnum::FLAP_LEFT)
            {
                if (cmd.flapState == FlapState::FLAP_CLOSE)
                {
                    flapLeftServo.close();
                }
                else if (cmd.flapState == FlapState::FLAP_OPEN)
                {
                    flapLeftServo.open();
                }
            }
            else if (cmd.flap == FlapServoEnum::FLAP_CENT)
            {
                if (cmd.flapState == FlapState::FLAP_CLOSE)
                {
                    closeCentFlap();
                }
                else if (cmd.flapState == FlapState::FLAP_OPEN)
                {
                    openCentFlap();
                }
            }
            else if (cmd.flap == FlapServoEnum::FLAP_RGHT)
            {
                if (cmd.flapState == FlapState::FLAP_CLOSE)
                {
                    flapRghtServo.close();
                }
                else if (cmd.flapState == FlapState::FLAP_OPEN)
                {
                    flapRghtServo.open();
                }
            }
        }
    }
}
