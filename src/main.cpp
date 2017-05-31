#include<Arduino.h>
#include<Screen.h>


Screen display(9, 10, 11, 12);  // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7 (new blue HalTec OLED)

int BUTTON1_PIN = 3;
int BUTTON2_PIN = 2;
int currentMenuItem = 0;
int currentMenuScreen = 0;

void setup() {

  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  display.showMenu(currentMenuScreen);

  Serial.begin(9600);
  Serial.print("Phone : ");
  Serial.println(display.getPhoneNumber());

}


void loop() {
  int buttonOneState = digitalRead(BUTTON1_PIN);
  int buttonTwoState = digitalRead(BUTTON2_PIN);

  currentMenuScreen = display.getMenuDisplay();

  if (buttonTwoState == HIGH) {
    bool calculate = true;

    if (calculate && currentMenuScreen == 1) {
      currentMenuScreen = 0;
      display.showMenu(currentMenuScreen);
      delay(200);
      calculate = false;
    }

    if (calculate && currentMenuScreen == 2) {
      display.drawNextPhoneItem();
      calculate = false;
    }

    if (calculate && currentMenuScreen == 0) {
      currentMenuScreen = currentMenuItem + 1;
      Serial.print("Changing Display to screen : ");
      Serial.println(currentMenuScreen);
      display.showMenu(currentMenuScreen);
      delay(200);
      calculate = false;
    }

  }

  if (buttonOneState == HIGH) {

    if (currentMenuScreen == 0) {
      currentMenuItem++;
      if (currentMenuItem > 2) currentMenuItem = 0;
      delay(200);
    }

    if (currentMenuScreen == 1) {
      display.toogleInterVal();
    }

    if (currentMenuScreen == 2) {
      display.tooglePhoneItem();
    }

  }

  display.showMenuItem(currentMenuItem);
  display.update();

//  delay(800);
}