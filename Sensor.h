#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

#define ZEIT 10

#define AUS 0
#define WEISS 1
#define ROT 2
#define GRUEN 3
#define BLAU 4

class Lichtsensor {
public:
  Lichtsensor();
  Lichtsensor(int pin_sen);

  int getWert();
  void update();

  byte helligkeit_high();
  byte helligkeit_low();

  int helligkeit;
  int _pin_sen;
};

// Farbsensor erbt die Methoden und Variablen von Lichtsensor, hat jedoch noch weitere
class Farbsensor : public Lichtsensor {
public:
  Farbsensor();
  Farbsensor(int pin_sen, int pin_R, int pin_G, int pin_B);

  void update();
  void setLED(int farbe);

  byte rot_prozent, gruen_prozent, blau_prozent;

private:
  int _pin_R, _pin_G, _pin_B;
};


#endif
