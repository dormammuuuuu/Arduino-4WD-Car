/*
Library for the motors control using L298N driver
Developer: Maksim Masalski
E-mail: maxxliferobot@gmail.com
*/


#include <Arduino.h>

class Motors
{
private:
    //motor 1
    int enA = 5;
    int in1 = 4;
    int in2 = 2;
    // motor 2
    int enB = 6;
    int in3 = 8;
    int in4 = 7;
public:
    void attach();
    void write(int, int);
};
