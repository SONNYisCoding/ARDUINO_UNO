/*
 Water wheel + time settings
 Product of: GECT company
 Create by Nguyễn Minh Triết
 date of manufacture: 10/3/2019
 ----THANKS FOR USING----
 */
 // Time settings
 #include <Wire.h> 
 const byte DS1307 = 0x68;//DS1307 address
 const byte NumberOfFields = 7; //The number of bytes of data will be read from DS1307
 int second, minute, hour, day, wday, month, year; //Declare time variables
 int week = 0;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  //Set time for the module
  setTime(8,0,0,1,12,5,19);//
  Serial.begin(9600);
}
void readDS1307(){
   Wire.beginTransmission(DS1307);
        Wire.write((byte)0x00);
        Wire.endTransmission();
        Wire.requestFrom(DS1307, NumberOfFields);
        
        second = bcd2dec(Wire.read() & 0x7f);
        minute = bcd2dec(Wire.read() );
        hour   = bcd2dec(Wire.read() & 0x3f); // 24 hour function.
        wday   = bcd2dec(Wire.read() );
        day    = bcd2dec(Wire.read() );
        month  = bcd2dec(Wire.read() );
        year   = bcd2dec(Wire.read() );
        year += 2000;    
}
//Change from BCD formart(Binary-Coded Demical) to Demical
int bcd2dec(byte num){
  return((num/10*16) + (num % 16));
}
//Change from Demical to BCD
int dec2bcd(byte num){
        return ((num/10 * 16) + (num % 10));
}
void printDigits(int digits){
    // Time components are separated by : 
    Serial.print(":");    
    if(digits < 10)
    Serial.print('0');
    Serial.print(digits);
}
void digitalClockDisplay(){
    // Digital clock display of the time
    Serial.print(hour);
    printDigits(minute);
    printDigits(second);
    Serial.print(" ");
    Serial.print(day);
    Serial.print(" ");
    Serial.print(month);
    Serial.print(" ");
    Serial.print(year); 
    Serial.println(); 
}
void setTime(byte hr, byte minutes, byte sec, byte wd, byte d, byte mth, byte yr){
        Wire.beginTransmission(DS1307);
        Wire.write(byte(0x00)); // reset pointer
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(minutes));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}
void loop() {
  // put your main code here, to run repeatedly:
  //Read data from the DS1307 module
  readDS1307();
  //Show the time to Serial Monitor
  digitalClockDisplay();
  delay(100);
}
