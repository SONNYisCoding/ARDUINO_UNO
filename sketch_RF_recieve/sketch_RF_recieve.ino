/*
 RF_recieve_cotrol
 Written by Sony,Jul 27 2018
 Arduino IDE 1.8.5
 */
const int dataSize = 500; //Size of data = 500
byte storeData[dataSize]; // Data store = dataSize(500);
#define ledPin 13 //Onboard LED
#define rfrecievePin A4 //Recieve board --- analogPin1
int maxSignalLength = 255; //Set max lenght of the signal = 255
int dataCounter = 0;
unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long timeDuring = 0;
const unsigned long upperThreshold = 100;
const unsigned long lowerThreshold = 80;
void setup() {
  // put your setup code here, to run once:
  // Code only run once, press the reset button Onboard to run again
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  while(analogRead(rfrecievePin) < 1){
    startTime = micros(); //Start the duringtime when rfrecievePin<1
  }
  digitalWrite(ledPin, HIGH);
  for(int i = 0; i < dataSize; i=i+2){
    dataCounter = 0;
    while(analogRead(rfrecievePin) > upperThreshold && dataCounter < maxSignalLength){
      dataCounter++;
    }
    storeData[i]=dataCounter;
    dataCounter=0;//reset the counter
    while(analogRead(rfrecievePin) < lowerThreshold && dataCounter < maxSignalLength){
      dataCounter++;
    }
    storeData[i+1]=dataCounter;
  }
  endTime = micros();
  timeDuring = endTime - startTime;
  digitalWrite(ledPin, LOW); // turn led OFF
  Serial.println("====================");
  Serial.print("Read duration ");
  Serial.print(timeDuring);
  Serial.println("microSec");
  Serial.println("====================");
  Serial.println("HIGH,LOW");
  delay(20);
  for(int i = 0; i < dataSize; i=i+2){
    Serial.print(storeData[i]);
    Serial.print(",");
    Serial.println(storeData[i+1]);
    delay(20);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  // Do Nothing
}
