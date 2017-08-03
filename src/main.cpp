#include<Arduino.h>
#include<Screen.h>
#include<TankSensor.h>

TankSensor sensor(8,7); // TankSensor(int SIGNAL_OUT_PIN, int SIGNAL_IN_PIN)
Screen display(9, 10, 11, 12);  // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7 (new blue HalTec OLED)

int BUTTON1_PIN = 3;
int BUTTON2_PIN = 2;
int currentMenuItem = 0;
int currentMenuScreen = -1;
long logoTimer = 0;
unsigned long lastState = 0;
unsigned long nextAlert = 0;

void pushedButton() {
  if ((lastState + 10000) < millis()) {
    Serial.println("Dont PUSH ME!");
    currentMenuScreen++;
    display.showMenu(currentMenuScreen);
  }
  lastState = millis();
}

void alert() {
  if (nextAlert < millis()) {
    nextAlert = millis() + (display.getInterval() * 864000000);
    Serial.print("INTERVAL : ");
    Serial.println(display.getInterval());
    Serial.println("UPS this is kinda shit!");
    Serial.println(millis());
    Serial.println(nextAlert);
  }
}


void setup() {

  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), pushedButton, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), pushedButton, RISING);

  display.showMenu(currentMenuScreen);

  Serial.begin(9600);
  Serial.print("Phone : ");
  Serial.println(display.getPhoneNumber());
  Serial.print("Pin : ");
  Serial.println(display.getPinCode());
  Serial.print("Pin Active : ");
  Serial.println(display.isPinCodeActive() ? "YES" : "NO");
  Serial.print("Current MenuScreen : ");
  Serial.println(currentMenuScreen);
  logoTimer = millis() + 4000;
  lastState = millis();

}


void loop() {
  int buttonOneState = digitalRead(BUTTON1_PIN);
  int buttonTwoState = digitalRead(BUTTON2_PIN);

  currentMenuScreen = display.getMenuDisplay();

  if (currentMenuScreen == -1) {
    if (logoTimer <= millis() ) {
       currentMenuScreen++;
       display.showMenu(currentMenuScreen);
    }
  }


  if (buttonTwoState == HIGH) {
    bool calculate = true;
    lastState = millis();
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

    if (calculate && currentMenuScreen == 3) {
      display.drawNextPinItem();
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
    lastState = millis();
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

    if (currentMenuScreen == 3) {
      display.tooglePinItem();
    }

  }

  display.showMenuItem(currentMenuItem);
  display.update();

  if (sensor.echo()) {
    alert();
  }

  if ((lastState + 10000) < millis()) {
      currentMenuScreen = -1;
      logoTimer = millis() + 4000;
      display.showMenu(currentMenuScreen);
  }

//  delay(800);
}
