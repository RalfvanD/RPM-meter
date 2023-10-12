#include <LiquidCrystal.h>
#include <CAN.h>

// Most cars support 11-bit adddress, others (like Honda),
// require 29-bit (extended) addressing, set the next line
// to false to use extended addressing
const bool useStandardAddressing = true;

// initialize the LCD library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// The values when it should display a message to shift up or down.
const int shiftUpRed = 2400, shiftUpYellow = 2100, shiftDownRed = 1400, shiftDownYellow = 1700;

void setup() {
  // Setup CAN bus
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // add filter to only receive the CAN bus ID's we care about
  if (useStandardAddressing) {
    CAN.filter(0x7e8);
  } else {
    CAN.filterExtended(0x18daf110);
  }

  // Setup LCD
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("Current RPM: ");

  // Setup LED Pins
  pinMode(A0, OUTPUT);// Red shift down LED
  pinMode(A1, OUTPUT);// Yellow shift down LED
  pinMode(A2, OUTPUT);// Green LED
  pinMode(A3, OUTPUT);// Green LED
  pinMode(A4, OUTPUT);// Green LED
  pinMode(A5, OUTPUT);// Yellow shift up LED
  pinMode(9, OUTPUT);// Red shift up LED

}

void loop() {
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // number of additional bytes
  CAN.write(0x01); // show current data
  CAN.write(0x0c); // engine RPM
  CAN.endPacket();

  // wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() < 3 ||          // correct length
         CAN.read() != 0x41 ||      // correct mode
         CAN.read() != 0x0c);       // correct PID

  float RPM = ((CAN.read() * 256.0) + CAN.read()) / 4.0;

  // Writing the RPM value to the screen
  // When the RPM is a three digit number the position needs to be adjusted.
  if(RPM < 1000){
    lcd.setCursor(13, 0);
  }
  else{
    lcd.setCursor(12, 0);
  }

  lcd.print(RPM);
  
  lcd.setCursor(0, 1);

  // Turning on the LEDs and showing a message to shift up at certain values.
  if (RPM < shiftDownRed) {
    lcd.print("Shift Down!");
    digitalWrite(A0, HIGH);
  }
  else if (RPM >= shiftDownRed && RPM <= shiftDownYellow){
    digitalWrite(A1, HIGH);
  }
  else if (RPM <= shiftUpRed && RPM >= shiftUpYellow){
    digitalWrite(A5, HIGH);
  }
  else if (RPM > shiftUpRed) {
    lcd.print("Shift Up!");
    digitalWrite(9, HIGH);
  }
  else{
    lcd.print("            ");
    digitalWrite(A2, HIGH);
    digitalWrite(A3, HIGH);
    digitalWrite(A3, HIGH);
  }

  // A delay so the RPM values can be a bit more readable
  delay(100);

  // Reset all LEDs back to low
  // The first are the analog pins, which also map to an integer, A0 => 14, A1 => 15, etc.
  for(int i = 14; i <= 20; i++){
    digitalWrite(i, LOW);
    delay(100);
  }
  
  //The final digital pin that we use can then be reset
  digitalWrite(9, LOW);
  
}
