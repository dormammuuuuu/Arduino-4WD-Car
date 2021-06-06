
// These are the necessary libraries.
#include "SR04.h"	//Distance Sensor Library
#include "MotorL298NRobotCar.h" //Motor Library
#include "LineSensorRobotCar.h" //Line Tracking Library
#include <SoftwareSerial.h> //Required Bluetooth Library
#include <Servo.h> //Servo Library
#include <IRremote.h> //IR Remote Library

//Pins for Distance Sensor
#define TRIG_PIN A1
#define ECHO_PIN A0

//Global Variables
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


//SET UP
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
  //Set the Servo position to 90 degrees.
  myservo.write(90);
  
  //Listen for Bluetooth input
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

  //If the app's arrow up is pressed, the robot car will move forward.
  if ( inData == "U") {
    Serial.println("UP");
    myMotors.write(-100, 100);
  }

  //If the app's arrow down is pressed, the robot car will move backward.
  else if ( inData == "D") {
    Serial.println("DOWN");
    myMotors.write(100, -100);
  }
  
  //If the app's arrow left is pressed, the robot car will turn left.
  else if ( inData == "L") {
    Serial.println("LEFT");
    myMotors.write(-255, 50);
  }
  
  //If the app's arrow right is pressed, the robot car will turn right.
  else if ( inData == "R") {
    Serial.println("RIGHT");
    myMotors.write(-50, 255);
  }

  //When the IR Control is pressed, the control is transferred to the IR Remote. If the user presses STOP in the app, the control can be returned to the app.
  else if ( inData == "I") {
    if (IrReceiver.decode(&results)) {
      if (results.value == 0xFFFFFFFF) { // if the value is equal to 0xFFFFFFFF
        results.value = key_value; // set the value to the key value
      }

      key_value = results.value; // store the value as key_value
      irrecv.resume(); // reset the receiver for the next code

      switch (results.value) {

		//If the user preses arrow up, the robot will move forward (SLOW)
        case 0xFF18E7:
          Serial.println("UP");
          myMotors.write(-100, 100); //Robot is running straight forward
          break;

		//If the user presses the number 2, the robot will move forward (FAST)
        case 0xFF629D:
          Serial.println("2");
          myMotors.write(-255, 255); //Robot is running straight forward
          break;
		  
		//If the user preses arrow down, the robot will move backward (SLOW)
        case 0xFF4AB5:
          Serial.println("DOWN");
          myMotors.write(100, -100);
          break;
		
		//If the user presses the number 8, the robot will move forward (FAST)
        case 0xFFA857:
          Serial.println("8");
          myMotors.write(255, -255); //Robot is running straight forward
          break;
		  
		//If the user presses arrow right, the robot will turn right (SLOW)
        case 0xFF5AA5:
          Serial.println("|>>");
          myMotors.write(-50, 100);
          break;

		//If the user presses the number 6, the robot will turn right (FAST)
        case 0xFFC23D:
          Serial.println("6");
          myMotors.write(-50, 255);
          break ;

		//If the user presses arrow left, the robot will turn left (SLOW)
        case 0xFF10EF:
          Serial.println("<<|");
          myMotors.write(-100, 50);
          break;
		
		//If the user presses the number 4, the robot will turn left (FAST)
        case 0xFF22DD:
          Serial.println("4");
          myMotors.write(-255, 50);
          break ;

		//When the user presses the asterisk button, the robot enters Obstacle Avoidance mode.
        case 0xFF6897:
          Serial.println("*");
          inData = "O";
          goto Obstacle;
          break ;
        
		//When the user presses the number sign button, the robot enters the Line Tracking mode.
        case 0xFFB04F:
          Serial.println("#");
          inData = "T";
          goto LineTracking;
          break ;
      }
      
      delay(100);
      myMotors.write(0, 0);
    }
  }
  
  //When the user presses the Obstacle Avoidance Mode in the app, the robot enters the Obstacle Avoidance mode
  else if ( inData == "O" ) {
    Obstacle:
    a = sr04.Distance();	//The Distance sensor will scan
    Serial.print(a);		//the distance of obstacles
    Serial.println("cm");	//in front of it.
	
    if (a < 20) {	//If the distance becomes less than 20, the code block below will be executed
      myMotors.write(0, 0);
		
      for (int i = 20; i <= 160; i++) { // The robot will scan the obstacle in front of it from 20 to 160Deg.
        myservo.write(i);
        a = sr04.Distance();
        Serial.print(a);
        Serial.println("cm");
        if (a > 25 && i < 90) {	//If the right side is clear, it will turn right.
          myMotors.write(100,-100);
          delay(500);
          myMotors.write(100, 100); //Robot turns right
          delay(500);
          break;
        }
        else if (a > 25 && i > 90) { //If the left side is clear, it will turn left.
          myMotors.write(100,-100);
          delay(500);
          myMotors.write(-100, -100); //Robot turns left
          delay(500);
          break;
        }
      }
    } else if (a > 20) {	//If there are no obstacles, the robot will move forward
      myMotors.write(-100, 100); //Robot is running straight forward
    }
    myservo.write(90);	 //Set the Servo position to 90 degrees.
  }
  
  //When the user presses the Line Trackin mode in the app, the robot enters the Line Tracking mode
  else if ( inData == "T" ) {
    LineTracking:
    myMotors.write(-100, 100); //Robot is going forward
    delay(250);
    Serial.println("#");
    Serial.print("left_light_value = ");
    Serial.println(myLineSensorA3.read());
    Serial.print("center_light_value = ");
    Serial.println(myLineSensorA4.read());
    Serial.print("right_light_value = ");
    Serial.println(myLineSensorA5.read());
    if (myLineSensorA4.read() == 0) { //If the middle sensor detects a black line, it will move forward.
      myMotors.write(-80, 80); //Robot is going forward
    } else {
      if (myLineSensorA3.read() == 0) { //If the left sensor detects black line, it will turn left until the middle sensor detects a black surface line.
        while (myLineSensorA4.read() == 1) {
          myMotors.write(-80, 0); //Robot turns left
        }
      } else {
        if (myLineSensorA5.read() == 0) { //If the right sensor detects black line, it will turn left right the middle sensor detects a black surface line.
          while (myLineSensorA4.read() == 1) {
            myMotors.write(0, 80); //Robot turns right
          }
        }
      }
    }
  } else {  //If the user presses STOP or do nothing, the robot will not move.
    myMotors.write(0, 0);
  }
}
