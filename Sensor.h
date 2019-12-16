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

  int _pin_sen;

private:

};

class Farbsensor : public Lichtsensor {
public:
  Farbsensor();
  Farbsensor(int pin_sen, int pin_R, int pin_G, int pin_B);

  void getWerte(int & helligkeit, int & prozent_rot, int & prozent_gruen, int & prozent_blau);
  void setFarbe(int farbe);

private:
  int _pin_R, _pin_G, _pin_B;
  int _wert_R, _wert_G, _wert_B;
};


#endif
