/*
Library for the line sensor T1000
Developer: Maksim Masalski
E-mail: maxxliferobot@gmail.com
*/

#include <Arduino.h>
class LineSensor
{
private:
    int pinLineSensor;
    int lineValue;
public:
    void attach(int);
    int read();
};
