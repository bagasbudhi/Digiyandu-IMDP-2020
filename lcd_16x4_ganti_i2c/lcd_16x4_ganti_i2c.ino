/* I2C LCD with Arduino example code. More info: https://www.makerguides.com */

// Include the libraries:
// LiquidCrystal_I2C.h: https://github.com/johnrickman/LiquidCrystal_I2C
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,4); // Change to (0x27,16,2) for 16x2 LCD.

void setup() {
  Wire.begin(8, 7); //SDA, SCL
  // Initiate the LCD:
  lcd.init();
  lcd.backlight();

  Wire.beginTransmission(0x08);
  Wire.write(0x23);
  byte busStatus = Wire.endTransmission();
  Serial.println(busStatus);

  Wire.requestFrom(0x08, 1);
  byte x = Wire.read();
  Serial.println(x, HEX);
  lcd.print(x, HEX);
}

void loop() {
  // Print 'Hello World!' on the first line of the LCD:
  lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
  lcd.print("Hello World!"); // Print the string "Hello World!"
  lcd.setCursor(2, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print("Bagas Budhi Permana");
  ///////////
//  lcd.setCursor(0, 0);
//  // print from 0 to 9:
//  for (int thisChar = 0; thisChar < 10; thisChar++) {
//    lcd.print(thisChar);
//    delay(500);
//    lcd.clear();
  }

  // set the cursor to (16,1):
//  lcd.setCursor(16, 1);
//  // set the display to automatically scroll:
//  lcd.autoscroll();
//  // print from 0 to 9:
//  for (int thisChar = 0; thisChar < 10; thisChar++) {
//    lcd.print(thisChar);
//    delay(500);
//  }
//  // turn off automatic scrolling
//  lcd.noAutoscroll();
//
//  // clear screen for the next loop:
  
