#include "Arduino.h"
#include "Sensor.h"

Lichtsensor::Lichtsensor() {
  _pin_sen = -1;
}

Lichtsensor::Lichtsensor(int pin_sen) {
  _pin_sen = pin_sen;
}

int Lichtsensor::getWert() {
  return analogRead(_pin_sen);
}





Farbsensor::Farbsensor() {
  _pin_R = -1;
  _pin_G = -1;
  _pin_B = -1;
}

Farbsensor::Farbsensor(int pin_sen, int pin_R, int pin_G, int pin_B) : Lichtsensor(pin_sen) {
  _pin_sen = pin_sen;
  _pin_R = pin_R;
  _pin_G = pin_G;
  _pin_B = pin_B;

  pinMode(_pin_sen, INPUT);
  pinMode(_pin_R, OUTPUT);
  pinMode(_pin_G, OUTPUT);
  pinMode(_pin_B, OUTPUT);

  setFarbe(WEISS);
}

void Farbsensor::getWerte(int & helligkeit, int & prozent_rot, int & prozent_gruen, int & prozent_blau) {
  setFarbe(ROT);
  delay(ZEIT);
  _wert_R = 1023 - getWert();

  setFarbe(GRUEN);
  delay(ZEIT);
  _wert_G = 1023 - getWert();

  setFarbe(BLAU);
  delay(ZEIT);
  _wert_B = 1023 - getWert();

  setFarbe(WEISS);
  delay(ZEIT);
  helligkeit = 1023 - getWert();

  prozent_rot  = _wert_R * 100L / ( _wert_R + _wert_G + _wert_B);
  prozent_gruen = _wert_G * 100L / ( _wert_R + _wert_G + _wert_B);
  prozent_blau = _wert_B * 100L / ( _wert_R + _wert_G + _wert_B);

}

void Farbsensor::setFarbe(int farbe) {
  switch (farbe) {
    case WEISS:
      digitalWrite(_pin_R, HIGH);
      digitalWrite(_pin_G, HIGH);
      digitalWrite(_pin_B, HIGH);
      break;

    case ROT:
      digitalWrite(_pin_R, HIGH);
      digitalWrite(_pin_G, LOW);
      digitalWrite(_pin_B, LOW);
      break;
    case GRUEN:
      digitalWrite(_pin_R, LOW);
      digitalWrite(_pin_G, HIGH);
      digitalWrite(_pin_B, LOW);
      break;
    case BLAU:
      digitalWrite(_pin_R, LOW);
      digitalWrite(_pin_G, LOW);
      digitalWrite(_pin_B, HIGH);
      break;
  }
}
