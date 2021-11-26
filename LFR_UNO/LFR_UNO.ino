/*
 Line Follower Robot + PID + distance from robot to obstacle
 Product of: GECT company
 Create by Nguyễn Minh Triết
 date of manufacture: 10/3/2019
 ----THANKS FOR USING----
 */
// Robot defines
#define NO________LINE 1
#define CROSS____LINES 2
#define JUNCTION_LINES 3
#define FOLLOWING_LINE 4
#define TURN______LEFT 5
#define TURN_____RIGHT 6
int mode = 0;
int maxwidthCrossLines = 3;
int crosslines_width = 3;
int count_crosslines = 0;
const int ledPin = LED_BUILTIN; //Pin 13
int count_loops = 0;
bool led_blink = LOW;
//Ultra sonic sensor setup
const int trigPin = 3;
const int echoPin = 4;
double distance = 0; // distance (use centimeters)
// Servo
#include <Servo.h>
const int ServoPin = 11;
Servo myServo;
// LF sensor settings
byte LFsensor0 = B00000000;
byte LFsensorT = B00000000;
// PID setting
float Kp = 35;
float Ki = 0;
float Kd = 15;
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;
// Motor settings
const int ena = 5;
const int in1 = 7;
const int in2 = 8;
const int enb = 6;
const int in3 = 9;
const int in4 = 10;
const double motorAConst = 1.0;
const double motorBConst = 0.8;
int currentSpeed = 0;
void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
//motor A
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
//motor B   
  pinMode(enb, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
//Line Following Sensor 
  DDRC = DDRC | B00000000;
//Ultra Sonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
//Servo 
  myServo.attach(ServoPin);
  myServo.write(85);
}
void read_Sensor_Values(){
  LFsensor0 = B10000000 | PINC;
  if(     LFsensor0 == B10000000){mode = CROSS____LINES; error = 0;} // 00000
  else if(LFsensor0 == B10011110){mode = TURN______LEFT; error = 0;} // 11110
  else if(LFsensor0 == B10011100){mode = TURN______LEFT; error = 0;} // 11100
  else if(LFsensor0 == B10001100){mode = FOLLOWING_LINE; error =-3;} // 01100
  else if(LFsensor0 == B10011000){mode = FOLLOWING_LINE; error =-2;} // 11000
  else if(LFsensor0 == B10010000){mode = FOLLOWING_LINE; error =-1;} // 10000
  else if(LFsensor0 == B10010100){mode = FOLLOWING_LINE; error =-1;} // 10100
  else if(LFsensor0 == B10011101){mode = FOLLOWING_LINE; error = 0;} // 11101
  else if(LFsensor0 == B10011001){mode = FOLLOWING_LINE; error = 0;} // 11001
  else if(LFsensor0 == B10010001){mode = FOLLOWING_LINE; error = 0;} // 10001
  else if(LFsensor0 == B10011011){mode = FOLLOWING_LINE; error = 0;} // 11011
  else if(LFsensor0 == B10001010){mode = FOLLOWING_LINE; error = 0;} // 11010
  else if(LFsensor0 == B10010011){mode = FOLLOWING_LINE; error = 0;} // 10011
  else if(LFsensor0 == B10010111){mode = FOLLOWING_LINE; error = 1;} // 10111
  else if(LFsensor0 == B10000001){mode = FOLLOWING_LINE; error = 1;} // 00001
  else if(LFsensor0 == B10000101){mode = FOLLOWING_LINE; error = 2;} // 00101
  else if(LFsensor0 == B10000011){mode = TURN_____RIGHT; error = 2;} // 00011
  else if(LFsensor0 == B10000111){mode = TURN_____RIGHT; error = 0;} // 00111
  else if(LFsensor0 == B10000110){mode = FOLLOWING_LINE; error = 3;} // 00110
  else if(LFsensor0 == B10001111){mode = TURN_____RIGHT; error = 0;} // 01111
  else if(LFsensor0 == B10000100){mode = JUNCTION_LINES; error = 0;} // 00100
  else if(LFsensor0 == B10011111){mode = NO________LINE; error = 0;} // 11111
}
//Caculate the PID controller
void caculate_PID(){
  P = error;
  I = I + error;
  D = error - previous_error;
  PID_value = (Kp*P) + (Ki*I) + (Kd*D);
  previous_error = error;
}
void Distance_caculate(){
  digitalWrite(trigPin, LOW); // set trig pin LOW
  delayMicroseconds(2);// wait for 2 microseconds
  digitalWrite(trigPin, HIGH);// set trig pin HIGH
  delayMicroseconds(10);//wait for 10 microseconds
  digitalWrite(trigPin, LOW);// set trig pin low again
  long duration = pulseIn(echoPin, HIGH);
  double microsecondsToCentimeters = duration/29.412/2;   // count distance of obstacle
  distance = microsecondsToCentimeters;
}
void LMotorController_PID(int robotSpeed, int minAbsSpeed){
  int leftSpeed = robotSpeed + PID_value;   
  int rightSpeed = robotSpeed - PID_value; 
  if (leftSpeed < 0){
    leftSpeed = constrain(leftSpeed, -1*minAbsSpeed, -160);
  }
  else {
    leftSpeed = constrain(leftSpeed, minAbsSpeed, 160);
  }
  int realLeftSpeed = map(abs(leftSpeed), 0, 160, minAbsSpeed, 160);
  if (rightSpeed < 0){
    rightSpeed = constrain(rightSpeed, -1*minAbsSpeed, -160);
  }
  else {
    rightSpeed = constrain(rightSpeed, minAbsSpeed, 160);
  }
  int realRightSpeed = map(abs(rightSpeed), 0, 160, minAbsSpeed, 160);
  digitalWrite(in3, leftSpeed > 0 ? HIGH : LOW);
  digitalWrite(in4, leftSpeed > 0 ? LOW : HIGH);
  digitalWrite(in1, rightSpeed > 0 ? HIGH : LOW);
  digitalWrite(in2, rightSpeed > 0 ? LOW : HIGH);
  analogWrite(ena, realRightSpeed * motorAConst);
  analogWrite(enb, realLeftSpeed *  motorBConst);
  currentSpeed = (realLeftSpeed + realRightSpeed)/2;  
}
void LMotorController_move(int robotSpeed, int minAbsSpeed){
  int direct = 1;
  if (robotSpeed < 0){
    direct = -1;
    robotSpeed = constrain(robotSpeed, -1*minAbsSpeed, -160);
    }
  else {
    robotSpeed = constrain(robotSpeed, minAbsSpeed, 160);    
    }
  if (robotSpeed == currentSpeed) return;
  int realSpeed = max(minAbsSpeed, abs(robotSpeed));
  digitalWrite(in1, robotSpeed > 0 ? HIGH : LOW);
  digitalWrite(in2, robotSpeed > 0 ? LOW : HIGH);
  digitalWrite(in3, robotSpeed > 0 ? HIGH : LOW);
  digitalWrite(in4, robotSpeed > 0 ? LOW : HIGH);
  analogWrite(ena, realSpeed * motorAConst);
  analogWrite(enb, realSpeed * motorBConst);
  currentSpeed = direct * realSpeed;  
}
void LMotorController_turnRight(int robotSpeed){
  digitalWrite(in1,  LOW);
  digitalWrite(in2,  LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4,  LOW);
  analogWrite(ena, robotSpeed * motorAConst);
  analogWrite(enb, robotSpeed * motorBConst);  
}
void LMotorController_roundRight(int robotSpeed, bool kick){
  digitalWrite(in1,  LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4,  LOW);
  if(kick){
    analogWrite(ena, 130);
    analogWrite(enb, 130);
  }
  analogWrite(ena, robotSpeed * motorAConst);
  analogWrite(enb, robotSpeed * motorBConst);
}
void LMotorController_turnLeft(int robotSpeed){
  digitalWrite(in1, HIGH);
  digitalWrite(in2,  LOW);
  digitalWrite(in3,  LOW);
  digitalWrite(in4,  LOW);
  analogWrite(ena, robotSpeed * motorAConst);
  analogWrite(enb, robotSpeed * motorBConst);  
}
void LMotorController_roundLeft(int robotSpeed, bool kick){
  digitalWrite(in1, HIGH);
  digitalWrite(in2,  LOW);
  digitalWrite(in3,  LOW);
  digitalWrite(in4, HIGH);
  if(kick){
    analogWrite(ena, 130);
    analogWrite(enb, 130);
  }
  analogWrite(ena, robotSpeed * motorAConst);
  analogWrite(enb, robotSpeed * motorBConst);
}
void LMotorController_stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 255);
  analogWrite(enb, 255);
}
void ledBlink() {
 count_loops ++;
 if (led_blink == 1000) {
   if (led_blink == LOW) {
     led_blink = HIGH;
   }
   else{
     led_blink = LOW;
   }     
   digitalWrite (ledPin, led_blink);    
   count_loops = 0;
 }
}
void loop() {
  // put your main code here, to run repeatedly:
  read_Sensor_Values();
  ledBlink();
  switch(mode){
    case JUNCTION_LINES:
      previous_error = error;
      if (count_crosslines == 2){  //If number of crosslines = 2 then robot trunleft
        /*LMotorController_stop();
        delay(500);*/
        LMotorController_turnLeft(200);
        delay(500);
      };
      if(count_crosslines == 1){ //If number of crosslines = 1 then count distance of obstacle     
        Distance_caculate();
        if (distance <= 25){
         /* LMotorController_stop();
          delay(500);*/
          LMotorController_turnLeft(200);
          delay(500);
        };
        if ((distance > 25) && (distance <= 45)) {
          /*LMotorController_stop();
          delay(500);*/
          LMotorController_turnRight(200);
          delay(500);
        };
        if (distance > 45) {
          /*LMotorController_stop();
          delay(500);*/
          LMotorController_turnRight(200);
          delay(500);
        };
      };
      count_crosslines = 0; 
      crosslines_width = 0;  
      break;
    case CROSS____LINES:
      LMotorController_stop();
      LMotorController_move(160, 50);
      LMotorController_move(150, 50);
      delay(5);
      previous_error = error;
      break;
    case NO________LINE:
      LMotorController_stop();
      LMotorController_roundLeft(100, true);
      delay(5);
      previous_error = error;
      break;
    case FOLLOWING_LINE:  
      caculate_PID();
      LMotorController_PID(160, 0);
      if (crosslines_width >= maxwidthCrossLines){  // count crosslines
        count_crosslines ++;
      }
      crosslines_width = 0;
      break;
    case TURN_____RIGHT:
      LMotorController_turnRight(170);
      previous_error = error;
      break;
    case TURN______LEFT:
      LMotorController_turnLeft(170);
      previous_error = error;
      break;         
  }
}
