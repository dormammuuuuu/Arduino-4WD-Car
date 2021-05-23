#include "SR04.h"
#include "MotorL298NRobotCar.h"
#include<Servo.h>
#define TRIG_PIN A1
#define ECHO_PIN A0

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
Servo myservo;
Motors myMotors;
int a;
int servoPosition;


void setup() {
  Serial.begin(9600);
  myMotors.attach();
  myservo.attach(A2);
}


void loop() {
  a = sr04.Distance();
  Serial.print(a);
  Serial.println("cm");

  if (a < 20) {
      //Robot is going backward myMotors.write(LEFT MOTOR, RIGHT MOTOR
      myMotors.write(0, 0);
      for (int j = 0; j < 40; j++){
        myMotors.write(150, -150);   
      }
      
      for (int i =-45; i <= 180; i++){
        myservo.write(i);
        a = sr04.Distance();
        Serial.print(a);
        Serial.println("cm");
        if(a > 20 && i < 90){
          myMotors.write(-50, -255); //Robot turns right
          
          break;
        }
         else if (a > 20 && i > 90){
          myMotors.write(-255, -50); //Robot turns left
          break;
         }
      }
      
      
  } else if (a > 20) {
    myMotors.write(-100, 100); //Robot is running straight forward
  } 
  myservo.write(45);

}
