/********************
 4x4 matrix keypad
 By Sony, Jul 19. 2018
 Connect:
  Row 1 - Digital 5
  Row 2 - Digital 6
  Row 3 - Digital 7
  Row 4 - digital 8
        --
  Col A - Digital 9
  Col B - Digital 10
  Col C - Digital 11
  Col D - Digital 12
********************/
int rowCounter = 0; // Set row counter
int columnCounter = 0; // Set column counter
int foundColumn = 0;
boolean foundCol = false;
int keyValue = 0;
int noKey = 0;
boolean readKey = false;
int debounce = 300; 
byte rows = 4; // 4 rows
byte columns = 4; // 4 columns
char keys[4][4] =
{
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'},
};
const int row1 = 5;
const int row2 = 6;
const int row3 = 7;
const int row4 = 8;
const int colA = 9;
const int colB = 10;
const int colC = 11;
const int colD = 12;
void setup() {
  // put your setup code here, to run once:
  pinMode(row1, OUTPUT);
  pinMode(row2, OUTPUT);
  pinMode(row3, OUTPUT);
  pinMode(row4, OUTPUT);
  pinMode(colA, INPUT_PULLUP);
  pinMode(colB, INPUT_PULLUP);
  pinMode(colC, INPUT_PULLUP);
  pinMode(colD, INPUT_PULLUP);
  Serial.begin(9600);
}
void keyPad(){
  if(noKey == 16){
    readKey = true;// dat so sanh lon cho
  }
  noKey = 0; 
  for(rowCounter = row1; rowCounter < row4 + 1; rowCounter++){
    scanRow();
    for(columnCounter = colA; columnCounter < colD + 1; columnCounter++){
      readColumn();
      if(foundCol == true){
        keyValue = (rowCounter - row1) + 4*(columnCounter - colA);
        rows = rowCounter - row1;
        columns = columnCounter - colA;
      }
    }
  }
  if(readKey == true && noKey == 15){
    Serial.println(keys[rows][columns]);// debug
  }
  readKey = false;//reset readKey 
  delay(debounce);//debounce
}
void scanRow(){
  for(int i = row1; i < row4 + 1; i++){
    digitalWrite(i, HIGH);//set all rows High
  }
  digitalWrite(rowCounter, LOW);
}
void readColumn(){
  foundColumn = digitalRead(columnCounter);// sai bien
  if(foundColumn == 0){
    foundCol = true;
  }else{
    foundCol = false;
    noKey = noKey + 1;
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  keyPad();
}
