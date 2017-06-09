#ifndef Screen_h
#define Screen_h
#include <U8glib.h>
#include <EEPROM.h>

class Screen {
public:
  U8GLIB_SH1106_128X64 u8g;
  Screen(int CLK_PIN, int MOSICS_PIN, int CS_PIN, int DC_PIN);
  void update();
  void draw();
  void showMenu(int index);
  void showMenuItem(int index);
  void toogleInterVal();
  void drawNextPhoneItem();
  void tooglePhoneItem();
  void tooglePinItem();
  void drawNextPinItem();
  String getPinCode();
  String getPhoneNumber();
  bool isPinCodeActive();
  int getInterval();
  int getMenuDisplay();
private:
  String int_array_to_string(int int_array[], int size_of_array);
  void loadConfig();
  void saveConfig();
  void drawLogo();
  void drawRootMenu();
  void drawIntervalMenu();
  void drawPhoneMenu();
  void drawPinMenu();
};

#endif
