/*
Library for the motors control using L298N driver
Developer: Maksim Masalski
E-mail: maxxliferobot@gmail.com
*/

#include <Arduino.h>
#include "MotorL298NRobotCar.h"

void Motors::attach()
{
    //Initialize all pins of the motordriver
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
}

void Motors::write(int a, int b)
{
    if (a > 0)
    {
        //Left motor rotate (by default project RoboRover M1 robot it is forward)
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        analogWrite(enA, a);
    }
    if (b > 0)
    {
        //Right motor rotate (by default project RoboRover M1 robot it is forward)
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        analogWrite(enB, b);
    }

    if (a == 0)
    {
        //Left motor stop
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        analogWrite(enA, 0);
    }
    if (b == 0)
    {
        //Right motor stop
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        analogWrite(enB, 0);
    }
    if (a < 0)
    {
        //Left motor rotate (by default project RoboRover M1 robot it is backward)
        a=abs(a);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        analogWrite(enA, a);
    }
    if (b < 0)
    {
        //Right motor rotate (by default project RoboRover M1 robot it is backward)
        b=abs(b);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        analogWrite(enB, b);
    }

}

