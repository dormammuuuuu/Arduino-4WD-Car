#include "SR04.h"
#include "MotorL298NRobotCar.h"
#include "LineSensorRobotCar.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include <IRremote.h>
#define TRIG_PIN A1
#define ECHO_PIN A0

SoftwareSerial HM10(0, 1); // RX = 2, TX = 3
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
Servo myservo;
LineSensor myLineSensorA3;
LineSensor myLineSensorA4;
LineSensor myLineSensorA5;
Motors myMotors;
int a;
int servoPosition;
int receiver_pin = 12;   //Connect the output pin of IR receiver at pin 4
unsigned long key_value = 0;
char appData;
String inData = "";

IRrecv irrecv(receiver_pin);
decode_results results;

void setup() {
  myLineSensorA3.attach(A5);
  myLineSensorA4.attach(A4);
  myLineSensorA5.attach(A3);
  irrecv.enableIRIn();
  myMotors.attach();
  myservo.attach(A2);
  Serial.begin(9600);
  HM10.begin(9600); // set HM10 serial at 9600 baud rate
}


void loop() {
  myservo.write(90);

  HM10.listen(); // listen the HM10 port

  while (HM10.available() > 0) { // if HM10 sends something then read
    appData = HM10.read();
    inData = String(appData); // save the data in string format
    Serial.write(appData);
  }

  if (Serial.available()) {     // Read user input if available.
    delay(10);
    HM10.write(Serial.read());
  }

  if ( inData == "U") {
    Serial.println("UP");
    myMotors.write(-100, 100);
  }

  else if ( inData == "D") {
    Serial.println("DOWN");
    myMotors.write(100, -100);
  }

  else if ( inData == "L") {
    Serial.println("LEFT");
    myMotors.write(-255, 50);
  }

  else if ( inData == "R") {
    Serial.println("RIGHT");
    myMotors.write(-50, 255);
  }

  else if ( inData == "I") {
    if (IrReceiver.decode(&results)) {
      if (results.value == 0xFFFFFFFF) { // if the value is equal to 0xFFFFFFFF
        results.value = key_value; // set the value to the key value
      }

      switch (results.value) {

        case 0xFF18E7:
          Serial.println("UP");
          myMotors.write(-100, 100); //Robot is running straight forward
          break;

        case 0xFF629D:
          Serial.println("2");
          myMotors.write(-255, 255); //Robot is running straight forward
          break;

        case 0xFF4AB5:
          Serial.println("DOWN");
          myMotors.write(100, -100);
          break;

        case 0xFFA857:
          Serial.println("8");
          myMotors.write(255, -255); //Robot is running straight forward
          break;

        case 0xFF38C7:
          Serial.println("STOP//OK");
          myMotors.write(0, 0);
          break;

        case 0xFF5AA5:
          Serial.println("|>>");
          myMotors.write(-50, 255); //Robot turns right
          break;

        case 0xFF10EF:
          Serial.println("<<|");
          myMotors.write(-255, 50); //Robot turns left
          break;
      }
      key_value = results.value; // store the value as key_value
      irrecv.resume(); // reset the receiver for the next code
      delay(100);
      myMotors.write(0, 0);
    }
  }
  else if ( inData == "O") {
    a = sr04.Distance();
    Serial.print(a);
    Serial.println("cm");
    if (a < 20) {
      //Robot is going backward myMotors.write(LEFT MOTOR, RIGHT MOTOR
      myMotors.write(0, 0);

      for (int i = 20; i <= 160; i++) {
        myservo.write(i);
        a = sr04.Distance();
        Serial.print(a);
        Serial.println("cm");
        if (a > 25 && i < 90) {
          myMotors.write(100,-100);
          delay(500);
          myMotors.write(100, 100); //Robot turns right
          delay(500);
          break;
        }
        else if (a > 25 && i > 90) {
          myMotors.write(100,-100);
          delay(500);
          myMotors.write(-100, -100); //Robot turns left
          delay(500);
          break;
        }
      }
    } else if (a > 20) {
      myMotors.write(-100, 100); //Robot is running straight forward
    }
    myservo.write(90);
  }

  else if ( inData == "T" ) {
    myMotors.write(-100, 100); //Robot is going forward
    delay(250);
    Serial.println("#");
    Serial.print("left_light_value = ");
    Serial.println(myLineSensorA3.read());
    Serial.print("center_light_value = ");
    Serial.println(myLineSensorA4.read());
    Serial.print("right_light_value = ");
    Serial.println(myLineSensorA5.read());
    if (myLineSensorA4.read() == 0) {
      myMotors.write(-80, 80); //Robot is going forward
    } else {
      if (myLineSensorA3.read() == 0) {
        while (myLineSensorA4.read() == 1) {
          myMotors.write(-80, -80); //Robot turns left
        }
      } else {
        if (myLineSensorA5.read() == 0) {
          while (myLineSensorA4.read() == 1) {
            myMotors.write(80, 80); //Robot turns right
          }
        }
      }
    }
  } else {
    myMotors.write(0, 0);
  }
}
