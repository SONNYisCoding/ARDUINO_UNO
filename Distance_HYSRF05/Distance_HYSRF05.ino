const int trigPin = 3;
const int echoPin = 4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
void distance(){
  unsigned long duration; // Time variables
  int distance;           // Distance variables
  digitalWrite(trigPin,0);   // Turn off trig pin
  delayMicroseconds(2);
  digitalWrite(trigPin,1);   // Pulse from trig pin
  delayMicroseconds(10);   // Pulse length 10 microSeconds
  digitalWrite(trigPin,0);   // Turn off trig pin
  duration = pulseIn(echoPin, HIGH);  
  // Count distance.
  distance = int(duration/2/29.412);
  // Print to Serial
  Serial.print(distance);
  Serial.println("cm");
  delay(1000);
}
void loop(){
  distance();
}
