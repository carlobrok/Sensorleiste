#ifndef Farbsensor_h
#define Farbsensor_h

#include "Arduino.h"

#define ZEIT 10

#define WEISS 0
#define ROT 1
#define GRUEN 2
#define BLAU 3


class Farbsensor {
public:
  Farbsensor();
  Farbsensor(int pin_sen, int pin_R, int pin_G, int pin_B);

  int getWert();
  void getWerte(int & helligkeit, int & prozent_rot, int & prozent_gruen, int & prozent_blau);
  void setFarbe(int farbe);

private:
  int _pin_sen;
  int _pin_R, _pin_G, _pin_B;
  int _wert_R, _wert_G, _wert_B;
};


#endif
