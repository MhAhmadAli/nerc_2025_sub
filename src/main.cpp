#include <Arduino.h>
#include <Wire.h>
#include "ServoControl.h"

void receiveEvent(int howMany)
{
    String data = "";
    while (Wire.available() > 0) // loop through all but the last
    {
        char chr = Wire.read(); // receive byte as a character
        data += String(chr);
    }

    Serial.println(data);

    if (data.indexOf("FT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        // queue.push({servo : &frontServo, pos : angle});
        moveServo(&frontServo, angle);
    }
    else if (data.indexOf("LT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        // queue.push({servo : &leftServo, pos : angle});
        moveServo(&leftServo, angle);
    }
    else if (data.indexOf("RT") > -1)
    {
        int angle = data.substring(data.indexOf(" ")).toInt();
        moveServo(&rightServo, angle);
    }
    else if (data.indexOf("BS") > -1) 
    {
        Serial.println("Moving base servo!");
        int angle = data.substring(data.indexOf(" ")).toInt();
        moveServo(&baseServo, angle);
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
}
