/*
Library for the line sensor TCRT5000
Developer: Maksim Masalski
E-mail: maxxliferobot@gmail.com
*/

#include <Arduino.h>
#include "LineSensorRobotCar.h"


void LineSensor::attach(int pinNumber)
{
    //Analog pins of the Arduino Uno can read digital signal if we use special number of the analog port
    //We compare analog port number with the digital port number of the analog pin
    if(pinNumber==A0)
        pinLineSensor=14;

    if(pinNumber==A1)
        pinLineSensor=15;

    if(pinNumber==A2)
        pinLineSensor=16;

    if(pinNumber==A3)
        pinLineSensor=17;

    if(pinNumber==A4)
        pinLineSensor=18;

    if(pinNumber==A5)
        pinLineSensor=19;

    pinMode(pinLineSensor,INPUT);
}

int LineSensor::read()
{
    //Digital read of the digital signal using analog inputs of the Arduino board
    //Result can be type int 0 or 1
    lineValue=digitalRead(pinLineSensor);
    return lineValue;
}
