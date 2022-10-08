 /*
  Line Follower Robot with PID Controller
    Create By Nguyễn Minh Triết
    Date: 25-02-2019
    Thanks For Using
*/

//-------------- Robot Defines

# define NO________LINE 0
# define CROSS____LINES 1
# define JUNCTION_LINES 2
# define FOLLOWING_LINE 3
# define TURNRIGHT_LINE 4
# define TURNLEFT__LINE 5

int mode = 0;
int widthCrossLines = 0;
int maxwidthCrossLines = 2;
int countCrossLines = 0;

const int ledPin = LED_BUILTIN; //Pin 13
int countLoops = 0;
bool ledState = LOW;
const int buttonPin = 9;


//-------------- LMotorController
const int _ena = 5;
const int _in1 = 7;
const int _in2 = 8;
const int _enb = 6;
const int _in3 = 9;
const int _in4 = 10;
const double _motorAConst = 1.0;
const double _motorBConst = 0.8;
int _currentSpeed = 0;

//-------------- Line Follower Sensor more to the Left is "0"

byte LFSensors0 = B00000000;
byte LFSensorsT = B00000000;

//-------------UltraSonic Sensor
const int trigPin = 3;
const int echoPin = 4;
double distance = 0; // distance with Centimetter

//-------------Servo Motor
#include <Servo.h>
const int servoPin = 11;
Servo myServo;

//-------------PID controller
float Kp=35;
float Ki=0;
float Kd=15;

float error=0, P=0, I=0, D=0, PIDvalue=0;
float previousError=0, previousI=0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  //---------------LMotorController

  pinMode(_ena, OUTPUT);
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
    
  pinMode(_enb, OUTPUT);
  pinMode(_in3, OUTPUT);
  pinMode(_in4, OUTPUT);
  
//-------------- Line Follower Sensor more to the Left is "0"
  
  DDRC = DDRC | B00000000;


//-------------- UltraSonic Sensor

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


//-------------- Servo Motor

  myServo.attach(servoPin);
  //delay(5000); // Delay 5 seconds for beginning
  myServo.write(85);
  
}

//--------------------------------------Read Line Follower Sensor

void readLFSensors(){
  LFSensors0 = B10000000 | PINC;  

  if(     LFSensors0 == B10000000){mode = CROSS____LINES; error = 0;}
  else if(LFSensors0 == B10011110){mode = TURNLEFT__LINE; error = 0;}
  else if(LFSensors0 == B10011100){mode = FOLLOWING_LINE; error =-3;}
  else if(LFSensors0 == B10001100){mode = FOLLOWING_LINE; error =-3;}
  else if(LFSensors0 == B10011000){mode = TURNLEFT__LINE; error = 0;}
  else if(LFSensors0 == B10010000){mode = TURNLEFT__LINE; error = 0;}
  else if(LFSensors0 == B10010100){mode = TURNLEFT__LINE; error = 0;}
  else if(LFSensors0 == B10011101){mode = FOLLOWING_LINE; error =-2;}
  else if(LFSensors0 == B10011001){mode = FOLLOWING_LINE; error =-1;}
  else if(LFSensors0 == B10010001){mode = FOLLOWING_LINE; error = 0;}
  else if(LFSensors0 == B10011011){mode = FOLLOWING_LINE; error = 0;}
  else if(LFSensors0 == B10001010){mode = FOLLOWING_LINE; error = 0;}
  else if(LFSensors0 == B10010011){mode = FOLLOWING_LINE; error = 1;}
  else if(LFSensors0 == B10010111){mode = FOLLOWING_LINE; error = 2;}
  else if(LFSensors0 == B10000001){mode = TURNRIGHT_LINE; error = 0;}
  else if(LFSensors0 == B10000101){mode = TURNRIGHT_LINE; error = 0;}
  else if(LFSensors0 == B10000011){mode = TURNRIGHT_LINE; error = 0;}
  else if(LFSensors0 == B10000111){mode = FOLLOWING_LINE; error = 3;}
  else if(LFSensors0 == B10000110){mode = FOLLOWING_LINE; error = 3;}
  else if(LFSensors0 == B10001111){mode = TURNRIGHT_LINE; error = 0;}
  else if(LFSensors0 == B10000100){mode = JUNCTION_LINES; error = 0;}
  else if(LFSensors0 == B10011111){mode = NO________LINE; error = 0;}
}
  
//------------------------------Calculate PID
void calculatePID(){
  P = error;
  I = I + error;
  D = error - previousError;
  PIDvalue = (Kp*P) + (Ki*I) + (Kd*D);
  previousError = error;
}
  
//-------------------------------Test Line Follower Sensor and PID value

void testLineFollowSensors(){ 
  readLFSensors();
  if(LFSensorsT != LFSensors0) {
    Serial.println(LFSensors0, BIN);
    Serial.print(" mode:  ");
    Serial.println(mode);
    Serial.print(" error:  ");
    Serial.println(error);
    Serial.println("------------------------");
  
    Serial.print(" P=  ");
    Serial.print(P);
    Serial.print(" Kp=  ");
    Serial.println(Kp);    
    Serial.print(" I=  ");
    Serial.println(I);
    Serial.print(" Ki=  ");
    Serial.println(Ki);    
    Serial.print(" D=  ");
    Serial.println(D);
    Serial.print(" Kd=  ");
    Serial.println(Kd);    
    Serial.print(" previousI=  ");
    Serial.println(previousI);  
    Serial.print(" PIDvalue=  ");
    Serial.println(PIDvalue);
    Serial.println("------------------------");    
    }
    LFSensorsT = LFSensors0;
}

//--------------------------------------Read UltraSonic Sensor

void DistanceObstacle(){
  digitalWrite(trigPin, LOW); // TrunOff trigger Pin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Transmit a pulse from trigger Pin
  delayMicroseconds(10); // width of pulse is 10 micro seconds
  digitalWrite(trigPin, LOW); // TrunOff trigger Pin
  long duration = pulseIn(echoPin, HIGH); // width of pulse that is received from echoPin
  double microsecondsToCentimeters = duration / 29.412 / 2;   // count distance of obstacle
  distance = microsecondsToCentimeters;
}

//---------------LMotorController
void LMotorController_move(int leftSpeed, int rightSpeed, int minAbsSpeed){
  if (leftSpeed < 0){
    leftSpeed = constrain(leftSpeed, -1*minAbsSpeed, -255);
    }
  else {
    leftSpeed = constrain(leftSpeed, minAbsSpeed, 255);
    }
  int realLeftSpeed = map(abs(leftSpeed), 0, 255, minAbsSpeed, 255);
  if (rightSpeed < 0){
    rightSpeed = constrain(rightSpeed, -1*minAbsSpeed, -255);
    }
  else {
    rightSpeed = constrain(rightSpeed, minAbsSpeed, 255);
    }
  int realRightSpeed = map(abs(rightSpeed), 0, 255, minAbsSpeed, 255);

  digitalWrite(_in3, leftSpeed > 0 ? HIGH : LOW);
  digitalWrite(_in4, leftSpeed > 0 ? LOW : HIGH);
  digitalWrite(_in1, rightSpeed > 0 ? HIGH : LOW);
  digitalWrite(_in2, rightSpeed > 0 ? LOW : HIGH);
  analogWrite(_ena, realRightSpeed * _motorAConst);
  analogWrite(_enb, realLeftSpeed * _motorBConst);
  
}
//---------------LMotorController_PID
void LMotorController_PID(int robotSpeed, int minAbsSpeed){
  int leftSpeed = robotSpeed + PIDvalue;   // If robot move to left sideways then PIDvalue < 0. So that "+" PIDvalue to decrease the leftSpeed
  int rightSpeed = robotSpeed - PIDvalue; // If robot move to right sideways then PIDvalue > 0. Sothat "-" PIDvalue to decerase the rightSpeed
  if (leftSpeed < 0){
    leftSpeed = constrain(leftSpeed, -1*minAbsSpeed, -255);
    }
  else {
    leftSpeed = constrain(leftSpeed, minAbsSpeed, 255);
    }
  int realLeftSpeed = map(abs(leftSpeed), 0, 255, minAbsSpeed, 255);
  if (rightSpeed < 0){
    rightSpeed = constrain(rightSpeed, -1*minAbsSpeed, -255);
    }
  else {
    rightSpeed = constrain(rightSpeed, minAbsSpeed, 255);
    }
  int realRightSpeed = map(abs(rightSpeed), 0, 255, minAbsSpeed, 255);

  digitalWrite(_in3, leftSpeed > 0 ? HIGH : LOW);
  digitalWrite(_in4, leftSpeed > 0 ? LOW : HIGH);
  digitalWrite(_in1, rightSpeed > 0 ? HIGH : LOW);
  digitalWrite(_in2, rightSpeed > 0 ? LOW : HIGH);
  analogWrite(_ena, realRightSpeed * _motorAConst);
  analogWrite(_enb, realLeftSpeed * _motorBConst);

  _currentSpeed = (realLeftSpeed + realRightSpeed)/2;  
}
 
//----------------
void LMotorController_move(int robotSpeed, int minAbsSpeed){
  int direct = 1;
  if (robotSpeed < 0){
    direct = -1;
    robotSpeed = constrain(robotSpeed, -1*minAbsSpeed, -255);
    }
  else {
    robotSpeed = constrain(robotSpeed, minAbsSpeed, 255);    
    }
  if (robotSpeed == _currentSpeed) return;

  int realSpeed = max(minAbsSpeed, abs(robotSpeed));

  digitalWrite(_in1, robotSpeed > 0 ? HIGH : LOW);
  digitalWrite(_in2, robotSpeed > 0 ? LOW : HIGH);
  digitalWrite(_in3, robotSpeed > 0 ? HIGH : LOW);
  digitalWrite(_in4, robotSpeed > 0 ? LOW : HIGH);
  analogWrite(_ena, realSpeed * _motorAConst);
  analogWrite(_enb, realSpeed * _motorBConst);
  
  _currentSpeed = direct * realSpeed;  
}

//----------------
void LMotorController_move(int robotSpeed){
  if (robotSpeed == _currentSpeed) return;
  robotSpeed = constrain(robotSpeed, -255, 255);
  
  digitalWrite(_in1, robotSpeed > 0 ? HIGH : LOW);
  digitalWrite(_in2, robotSpeed > 0 ? LOW : HIGH);
  digitalWrite(_in3, robotSpeed > 0 ? HIGH : LOW);
  digitalWrite(_in4, robotSpeed > 0 ? LOW : HIGH);
  analogWrite(_ena, abs(robotSpeed) * _motorAConst);
  analogWrite(_enb, abs(robotSpeed) * _motorBConst);
  
  _currentSpeed = robotSpeed;  
}

//----------------
void LMotorController_turnLeft(int robotSpeed){
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW);
  digitalWrite(_in4, LOW);
  analogWrite(_ena, robotSpeed * _motorAConst);
  analogWrite(_enb, robotSpeed * _motorBConst);  
}

//----------------
void LMotorController_roundLeft(int robotSpeed, bool kick){
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW);
  digitalWrite(_in4, HIGH);
  
  if (kick){
    analogWrite(_ena, 255);
    analogWrite(_enb, 255);
    delay(1); 
    }
  
  analogWrite(_ena, robotSpeed * _motorAConst);
  analogWrite(_enb, robotSpeed * _motorBConst);  
}

//----------------
void LMotorController_turnRight(int robotSpeed){
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, HIGH);
  digitalWrite(_in4, LOW);
  analogWrite(_ena, robotSpeed * _motorAConst);
  analogWrite(_enb, robotSpeed * _motorBConst);  
}


//----------------
void LMotorController_roundRight(int robotSpeed, bool kick){
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, HIGH);
  digitalWrite(_in3, HIGH);
  digitalWrite(_in4, LOW);
  
  if (kick){
    analogWrite(_ena, 255);
    analogWrite(_enb, 255);
    delay(100); 
    }
  
  analogWrite(_ena, robotSpeed * _motorAConst);
  analogWrite(_enb, robotSpeed * _motorBConst);  
}

//----------------
void LMotorController_stop(){
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW);
  digitalWrite(_in4, LOW);
  analogWrite(_ena, 255);
  analogWrite(_enb, 255);
  
  _currentSpeed = 0;
  
}

//---------------Led Blink Control with no delay-------
void ledBlink() {
 countLoops ++;
 if (countLoops == 1000) {
   if (ledState == LOW) {
     ledState = HIGH;
   }
   else{
     ledState = LOW;
   }     
   digitalWrite (ledPin, ledState);    
   countLoops = 0;
 }
}

//---------------Line Follower RoBot PID Control

void loop() {
  readLFSensors();
  ledBlink();
  switch (mode){
    case JUNCTION_LINES:
      //LMotorController_stop();      
      previousError = error;
      if (countCrossLines == 2){  //If number of crosslines = 2 then robot trunleft           
        LMotorController_turnLeft(255);
      };
      if(countCrossLines == 1){ //If number of crosslines = 1 then count distance of obstacle         
        DistanceObstacle();
        if (distance <= 25){
        LMotorController_stop();
        delay(5000);           
          LMotorController_turnLeft(255);
        };
        if ((distance > 25) && (distance <= 45)) {
          LMotorController_turnRight(255);
          delay(500);
        };
        if (distance > 45) {

          LMotorController_turnRight(255);
          delay(500);
        };
      };
      countCrossLines = 0; // Reset countCrossLines 
      widthCrossLines = 0; // reset widthCrossLines
      break;

    case CROSS____LINES:
      widthCrossLines ++;
      //LMotorController_stop();
      LMotorController_move(200, 50);
      LMotorController_move(150, 50);
      delay(10);
      previousError = error;
  
      break;

    case NO________LINE:
      LMotorController_stop();
      LMotorController_roundLeft(150, true);
      delay(5);
      previousError = error;
      break;

    case FOLLOWING_LINE:  
      calculatePID();
      LMotorController_PID(220, 0);
      //testLineFollowSensors();
      if (widthCrossLines >= maxwidthCrossLines){  // count crosslines
        countCrossLines ++;
      }
      widthCrossLines = 0; // reset widthCrossLines
      break;

    case TURNRIGHT_LINE:
      LMotorController_turnRight(255);
      previousError = error;
      //delay(100);
      break;

    case TURNLEFT__LINE:
      LMotorController_turnLeft(255);
      //delay(100);
      previousError = error;
      break;         
  }
}

