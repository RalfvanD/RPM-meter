#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int RPM = 800;
bool RPMincrease = true;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  

  
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Current RPM: ");
  
  if(RPM < 1000){
    lcd.setCursor(13, 0);
  }
  else{
    lcd.setCursor(12, 0);
  }

  lcd.print(RPM);
  
  lcd.setCursor(0, 1);

  
  if (RPM < 800) {
    lcd.print("Shift Down!");
  }
  else if (RPM > 2300) {
    lcd.print("Shift Up!");
  }
  else{
    lcd.print("            ");
  }

  
  if (RPMincrease){
    RPM += 5;
  }
  else{
    RPM -= 5;
  }

  if(RPM > 2500){
    RPMincrease = false;
  }
  if(RPM < 500){
    RPMincrease = true;
  }
}
