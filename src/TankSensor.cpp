#include <TankSensor.h>

int _in_pin;
int _out_pin;

TankSensor::TankSensor(int SIGNAL_OUT_PIN, int SIGNAL_IN_PIN){
  _in_pin = SIGNAL_IN_PIN;
  _out_pin = SIGNAL_OUT_PIN;
  pinMode(_in_pin, INPUT);
  pinMode(_out_pin, OUTPUT);
}


bool TankSensor::echo() {
    digitalWrite(_out_pin, HIGH);    
    return (digitalRead(_in_pin) == HIGH);
}
