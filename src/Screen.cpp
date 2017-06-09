#include <Screen.h>
#include <Arduino.h>

#define CONFIG_START 0

int _menuIndex = -1;
int _menuItemIndex = 0;
int _interval = 1;
int _phoneNumberItem = 0;
int _pinCodeItem = 0;
int _phoneNumber[12];
int _pinCode[4];
bool _pinActivated = true;

String menus[] = {"INTERVAL", "PHONE", "PIN"};

struct LocalConfig {
  int number[12];
  int pinCode[4];
  int interval;
  byte pinActivated;
} myConfig = { {0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0}, 3, 1};


Screen::Screen(int CLK_PIN, int MOSICS_PIN, int CS_PIN, int DC_PIN):u8g(CLK_PIN, MOSICS_PIN, CS_PIN, DC_PIN) {
  u8g.setFont(u8g_font_unifont);
  loadConfig();
}

int Screen::getMenuDisplay() {
  return _menuIndex;
}

void Screen::loadConfig(){
  EEPROM.get(CONFIG_START, myConfig);

  for (int index=0;index<12;index++) {
    _phoneNumber[index] = myConfig.number[index];
  }

  for (int index=0;index<4;index++) {
    _pinCode[index] = myConfig.pinCode[index];
  }

  _interval = myConfig.interval;
  _pinActivated = (myConfig.pinActivated == 0) ? false : true;

}

void Screen::saveConfig(){
    myConfig.interval = _interval;
    myConfig.pinActivated = (_pinActivated) ? 1 : 0;

    for (int index=0;index<12;index++) {
      myConfig.number[index] = _phoneNumber[index];
    }

    for (int index=0;index<4;index++) {
      myConfig.pinCode[index] = _pinCode[index];
    }

    EEPROM.put(CONFIG_START,myConfig);
}


void Screen::update() {
  // picture loop
   u8g.firstPage();
   do {
     draw();
   } while( u8g.nextPage() );

   // rebuild the picture after some delay
}

void Screen::showMenuItem(int index) {
    _menuItemIndex = index;
}

void Screen::showMenu(int index) {
  _menuIndex = index;
  if (index == -1) {
    drawLogo();
  } else if (index == 0) {
    drawRootMenu();
  } else if (index == 1) {
    drawIntervalMenu();
  } else if (index == 2) {
    drawPhoneMenu();
  } else if (index == 3) {
    drawPinMenu();
  } else {
    _menuIndex = 0;
    drawRootMenu();
  }
}

void Screen::tooglePhoneItem() {
  _phoneNumber[_phoneNumberItem]++;
  if (_phoneNumber[_phoneNumberItem] > 9) {
    _phoneNumber[_phoneNumberItem] = 0;
  }
  saveConfig();
  delay(200);
}

void Screen::drawNextPhoneItem(){
   _phoneNumberItem++;
   if (_phoneNumberItem == 12) {
      _menuIndex = 0;
      _phoneNumberItem = 0;
      Serial.println(int_array_to_string(_phoneNumber, 12));
      saveConfig();
   }
   delay(200);
}

bool Screen::isPinCodeActive() {
    return _pinActivated;
}

void Screen::tooglePinItem() {
  if (_pinCodeItem < 4) {
    _pinCode[_pinCodeItem]++;
  } else {
    _pinActivated = !_pinActivated;
  }
  if (_pinCode[_pinCodeItem] > 9) {
    _pinCode[_pinCodeItem] = 0;
  }
  saveConfig();
  delay(200);
}

void Screen::drawNextPinItem(){
   _pinCodeItem++;
   if (_pinCodeItem == 5) {
      _menuIndex = 0;
      _pinCodeItem = 0;
      Serial.println(int_array_to_string(_pinCode, 4));
      saveConfig();
   }
   delay(200);
}


void Screen::toogleInterVal() {
    _interval++;
    if (_interval > 9) {
      _interval = 1;
    }
    saveConfig();
    delay(200);
}

void Screen::drawPinMenu() {
  int offset = 16;
  int posX=0;
  int posY=10;
  u8g.setPrintPos(posX, posY);
  u8g.print("PIN code");
  posY = posY + offset;
  u8g.setPrintPos(posX, posY);
  for (int i =0; i < 4; i++) {
    u8g.setPrintPos(posX, posY);
    u8g.print(_pinCode[i]);
    posX = posX + 8;
  }
  if (_pinCodeItem < 4) {
    u8g.drawHLine(_pinCodeItem * 8, posY + 5, 8);
  }

  posY = posY + 25;
  posX = 0;

  u8g.setPrintPos(posX, posY);
  u8g.print("Enabled :");
  posX = posX + 80;
  u8g.setPrintPos(posX, posY);
  u8g.print((_pinActivated) ? "YES" : "NO");

  if (_pinCodeItem == 4) {
    u8g.drawHLine(posX, posY + 5, (_pinActivated) ? 3 * 8 :2 * 8);
  }

}

void Screen::drawPhoneMenu() {
  int offset = 16;
  int posX=0;
  int posY=10;
  u8g.setPrintPos(posX, posY);
  u8g.print("SMS number");
  posY = posY + offset;
  u8g.setPrintPos(posX, posY);
  for (int i =0; i < 12; i++) {
    u8g.setPrintPos(posX, posY);
    u8g.print(_phoneNumber[i]);
    posX = posX + 8;
  }
  u8g.drawHLine(_phoneNumberItem * 8, posY + 5, 8);

}

void Screen::drawIntervalMenu() {
  int offset = 16;
  int posX=0;
  int posY=10;
  u8g.setPrintPos(posX, posY);
  u8g.print("Notifications");
  posY = posY + offset;
  u8g.setPrintPos(posX, posY);
  u8g.print("Days [1-9] : ");
  u8g.setPrintPos(posX+100, posY);
  u8g.print(_interval);
  u8g.drawHLine(posX+100, posY + 5, 8);
}

void Screen::drawLogo() {
  int offset = 16;
  int posX=0;
  int posY=offset;
  u8g.setPrintPos(posX, posY);
  u8g.setFont(u8g_font_10x20);
  u8g.print("   CINTIX");
  posY = posY + 22;
  posX = 0;
  u8g.setPrintPos(posX, posY);
  u8g.setFont(u8g_font_6x10);
  u8g.print("  Michael Martinsen");
  posY = posY + 15;
  posX = 0;
  u8g.setPrintPos(posX, posY);
  u8g.print("    www.cintix.dk");
}

void Screen::drawRootMenu() {

  int offset = 16;
  int posX=0;
  int posY=offset;
  u8g.setFont(u8g_font_unifont);

  for (int i=0; i< 3; i++) {
    //u8g.drawHLine(0, posY+3, 128);
    if (i == _menuItemIndex) {
      posX = 0;
      u8g.setPrintPos(posX, posY);
      u8g.print(">");
      posX=8;
    } else {
      posX=8;
    }
    u8g.setPrintPos(posX, posY);
    u8g.print(menus[i]);
    posY = posY + offset+3;
  }
}

String Screen::int_array_to_string(int int_array[], int size_of_array) {
  String buffer = "";
  for (int index = 0; index < size_of_array; index++)
    buffer += int_array[index];
  return buffer;
}

String Screen::getPinCode(){
  return int_array_to_string(_pinCode, 4);
}

String Screen::getPhoneNumber(){
  return int_array_to_string(_phoneNumber, 12);
}

int Screen::getInterval(){
  return _interval;
}

void Screen::draw() {
  showMenu(_menuIndex);
}
