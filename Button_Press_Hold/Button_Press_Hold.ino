byte button1 = 5;
byte button2 = 6;
byte button3 = 7;
String button_1 = "NoPress";
boolean buttonState1 = LOW;
unsigned long timePress = 0;
long timeBegin = 0;
long timePress1 = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  Serial.begin(9600);
}
void Key(){
  buttonState1 = digitalRead(button1);
  button_1 = "NoPress";
  if(buttonState1 == LOW){
    timeBegin = millis();
    while(buttonState1 == LOW){
      timePress1 = millis();
      buttonState1 = digitalRead(button1);
    }
    timePress1 = timePress1 - timeBegin;
    delay(100);
    if(timePress1>150 && timePress1 < 2000){
      button_1 = "Press";
    }
    if(timePress1>=2000){
      button_1 = "Hold";
    }    
    Serial.println(button_1);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  Key();
}
