#include "GSM.h"
#include "SoftwareSerial.h"

bool debugEnabled = false;

GSM::GSM(int RX, int TX):gsm800(RX,TX) {
  Serial.begin(9600);
  while(!Serial);
  while (!gsm800);
}

void GSM::enableDebug(bool enable) {
  debugEnabled = enable;
}

void GSM::connect() {
  connect("");
}

void GSM::connect(String pinCode){
  if (debugEnabled) {
    if (pinCode.length() > 0) {
        printDebug("Connecting to GMS modem with PIN " + pinCode);
    } else {
        printDebug("Connecting to GMS modem");
    }
  }

  sendCommand("ATTTTTTTTTTTTTTTTTTTTTTTTTTTT");
  waitForResult("OK");

  if (pinCode.length() > 0) {
    sendCommand("AT+CPIN=" + pinCode);
    waitForResult("READY", "ERROR");
  }

  sendCommand("AT+CMGF=1");
  waitForResult("OK", "ERROR");

  sendCommand("AT+CSCS=\"8859-1\"");
  waitForResult("OK", "ERROR");

}

void GSM::sendCommand(String command) {
  if (debugEnabled) printDebug(command);
  gsm800.println(command); //bytes allocation for the GSM modem
  delay(100);
}

void GSM::sms(String phone, String message) {
  String convertedMessage = filterInsertSpecialChar(message);

  if (debugEnabled) {
    Serial.print("Sending SMS to ");
    Serial.println(phone);
    Serial.print("With content : ");
    Serial.println(message);
  }

  gsm800.print("AT+CMGS=\"+");
  gsm800.print(phone);
  gsm800.println("\"");

  waitForResult(">", "ERROR");

  gsm800.println(message);
  waitForResult(">", "ERROR");

  gsm800.write(0x1a); // ctrl+Z character
  waitForResult("CMGS", "ERROR");
}

String GSM::filterInsertSpecialChar(String message) {
  for (int index=0; index <= message.length(); index++){
    message.replace("ae",ae); // æ
    message.replace("AE",AE); // Æ
    message.replace("oe",oe); // ø
    message.replace("OE",OE); // Ø
    message.replace("aa",aa); // å
    message.replace("AA",AA); // Å
  }
  return message;
}

void GSM::printDebug(String message) {
  Serial.print("[DEBUG] - ");
  Serial.println(message);
}

void GSM::waitForResult(String result) {
  waitForResult(result, "");
}

void GSM::waitForResult(String result, String orResult){
  char buffer[60];
  int bufferSize = 0;
  char resultBuffer[10];
  char resultBuffer2[10];
  bool useOr = false;
  
  result.toCharArray(resultBuffer,10);

  if (orResult.length() > 0) {
    orResult.toCharArray(resultBuffer2,10);
    useOr = true;
  }

  if (!useOr) delete[] resultBuffer2;

  while(gsm800.available()) {
    if (bufferSize > sizeof(buffer)) break;

    buffer[bufferSize] = gsm800.read();
    bufferSize++;

// ------------------------------------------
    if (strstr(buffer,resultBuffer)) {
      if (debugEnabled) {
        printDebug(buffer);
      }
      break;
    }

// ------------------------------------------
    if (useOr && strstr(buffer, resultBuffer2)) {
      if (debugEnabled) {
        printDebug(buffer);
      }
      break;
    }

  }

  delete[] buffer;
  delete[] resultBuffer;

}
