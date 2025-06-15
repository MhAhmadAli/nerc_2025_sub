#include <Arduino.h>
#include <CircularBuffer.hpp>
#include <Wire.h>
#include "ServoControl.h"

typedef struct
{
    char servo;
    int pos;
} Cmd;

CircularBuffer<Cmd, 10> queue;

void receiveEvent(int howMany)
{
    String data = "";
    while (Wire.available() > 0) // loop through all but the last
    {
        char chr = Wire.read(); // receive byte as a character
        data += String(chr);
    }

    Serial.println(data);

    // f -> frontServo
    // l -> leftServo
    // r -> rightServo
    // b -> baseServo
    if (data.indexOf("FT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        queue.push({servo : 'f', pos : angle});
        // moveServo(&frontServo, angle);
    }
    else if (data.indexOf("LT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        queue.push({servo : 'l', pos : angle});
        // moveServo(&leftServo, angle);
    }
    else if (data.indexOf("RT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        queue.push({servo : 'r', pos : angle});
        // moveServo(&rightServo, angle);
    }
    else if (data.indexOf("BS") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        queue.push({servo : 'b', pos : angle});
        // moveServo(&baseServo, angle);
    }
}

void setup()
{
    Wire.begin(4); // join i2c bus with address #4
    Wire.onReceive(receiveEvent);
    Serial.begin(9600);

    initServos();
}

void loop()
{
    while (!queue.isEmpty())
    {
        Cmd cmd = queue.pop();

        if (cmd.servo == 'f') {
            moveServo(&frontServo, cmd.pos);
        } else if (cmd.servo == 'l') {
            moveServo(&leftServo, cmd.pos);
        } else if (cmd.servo == 'r') {
            moveServo(&rightServo, cmd.pos);
        } else if (cmd.servo == 'b') {
            moveServo(&baseServo, cmd.pos);
        }
    }
}
