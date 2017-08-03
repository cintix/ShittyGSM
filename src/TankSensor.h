
#ifndef TankSensor_h
#define TankSensor_h
#include <Arduino.h>

class TankSensor {
public:
  TankSensor(int SIGNAL_OUT_PIN, int SIGNAL_IN_PIN);
  bool echo();
};

#endif
