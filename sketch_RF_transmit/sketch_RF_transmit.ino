#define rfTransmitPin 4  //RF Transmitter pin --- digital pin 4
#define ledPin 13        //Onboard LED
const int codeSize = 25;      
int codeToTransmit[codeSize]; 
int doorStop[]={2,4,4,4,4,4,2,4,4,4,4,4,2,4,2,2,2,2,4,4,2,2,2,2,3}; //The Remote that will turn the door Stop
int doorDown[]={2,4,4,4,4,4,2,4,4,4,4,4,2,4,2,2,4,4,2,2,2,2,2,2,3}; //The Remote that will turn the door Down
int doorUp[]={2,4,4,4,4,4,2,4,4,4,4,4,2,4,2,2,2,2,2,2,2,2,4,4,3}; //The Remote that will turn the door Up
int doorLock[]={2,4,4,4,4,4,2,4,4,4,4,4,2,4,2,2,2,2,2,2,4,4,2,2,3}; //The Remote that will turn the door Lock
int codeToggler = 0;  
int timeDelay=112;      
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Connect to the Arduino COM3
  pinMode(ledPin, OUTPUT);
  pinMode(rfTransmitPin, OUTPUT); // The rfTransmitPin is OUTPUT
  // Blink the Onboard LED
  digitalWrite(ledPin,HIGH);
  delay(3000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  toggleCode();
  transmitCode(); // Transmit code to transmit
  delay(2000);
}
void toggleCode(){
  if(codeToggler){
    for(int i = 0; i < codeSize; i++){
      codeToTransmit[i] = doorStop[i];
    }
  }else{
    for(int i = 0; i < codeSize; i++){
      codeToTransmit[i] = doorDown[i];
    }
  }  
}
void transmitCode(){
  digitalWrite(ledPin, HIGH);// The Led turn on
  // Set highLength and lowLength
  int highLength = 0;
  int lowLength = 0;
  //Write the 6 code
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < codeSize; j++){
      switch(codeToTransmit[i]){
        case 1:{ //SH + LL
          highLength = 2;
          lowLength = 8;
          break;
        }
        case 2:{ //LH + SL
          highLength = 8;
          lowLength = 2;
          break;
        }
        case 3:{ //SH + SL
          highLength = 2;
          lowLength = 2;
          break;
        }
        case 4:{ //LH + LL
          highLength = 8;
          lowLength = 8;
          break;
        }
        case 5:{ //LH + VLL
          highLength = 8;
          lowLength = 133;
          break;
        }
        case 6:{ //SH + VLL
          highLength = 2;
          lowLength = 133;
          break;
        }
      }  
        digitalWrite(rfTransmitPin, HIGH);
        delayMicroseconds(highLength*timeDelay);
        digitalWrite(rfTransmitPin, LOW);
        delayMicroseconds(lowLength*timeDelay);
    }
  }    
  digitalWrite(ledPin, LOW);// Turn off Onboard LED
}
void loop() {
  // put your main code here, to run repeatedly:
  // Do nothing Here
}
