#include <Servo.h>// open servo control library
#define servoPin 11
Servo myServo;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(servoPin);
  myServo.write(0);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*for(int i = 0; i<4; i++){
    if(i = 1){
      myServo.write(180);
      delay(2000);
      myServo.write(0);
      delay(2000);
      myServo.write(180);
      delay(2000);
      myServo.write(0);
      delay(2000);
      Serial.println('1');
    }*/
      myServo.write(180);
      delay(2000);
      myServo.write(0);
      delay(2000);
      myServo.write(180);
      delay(2000);
      myServo.write(0);
      delay(2000);
      Serial.println('2');
    
    /*if(i = 3){
      myServo.write(45);
      delay(2000);
      myServo.write(45+45);
      delay(2000);
      myServo.write(0);
      delay(2000);
      Serial.println('3');
    }*/
  }

