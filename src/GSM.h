#ifndef GSM_h
#define GSM_h
#include "Arduino.h"
#include "SoftwareSerial.h"

const String ae = String(char(230));  // æ
const String AE = String(char(198));  // Æ
const String oe = String(char(248));  // ø
const String OE = String(char(216));  // Ø
const String aa = String(char(229));  // å
const String AA = String(char(197));  // Å


class GSM {
  public:
    GSM(int RX, int TX);
    void connect();
    void connect(String pinCode);
    void sms(String phone, String message);
    void enableDebug(bool enable);
  private:
    SoftwareSerial gsm800;
    int pinCode;
    void waitForResult(String result);
    void waitForResult(String result, String orResult);
    void printDebug(String message);
    void sendCommand(String command);
    String filterInsertSpecialChar(String message);
};




#endif
