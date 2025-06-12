#include <Arduino.h>
#include <Wire.h>
#include "ServoControl.h"

void receiveEvent(int howMany)
{
    String data = "";
    while (Wire.available()) // loop through all but the last
    {
        char c = Wire.read(); // receive byte as a character
        Serial.print(c);      // print the character
        data += String(c);
    }
    int x = Wire.read(); // receive byte as an integer
    data += String(x);
    Serial.println(data);   // print the integer
}

void setup()
{
    Wire.begin(4); // join i2c bus with address #4
    Wire.onReceive(receiveEvent);
    Serial.begin(9600);
    // mainBoard.begin(2400);
    initServos();
}

void loop()
{
    // if (mainBoard.available())
    // {
    //     String data = mainBoard.readStringUntil('\n');
    // }
}
