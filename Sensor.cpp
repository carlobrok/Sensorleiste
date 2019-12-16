#include "Arduino.h"
#include "Sensor.h"

Lichtsensor::Lichtsensor() {
  _pin_sen = -1;
  helligkeit = 0;
}

Lichtsensor::Lichtsensor(int pin_sen) {
  _pin_sen = pin_sen;
}

int Lichtsensor::getWert() {
  return analogRead(_pin_sen);
}

void Lichtsensor::update() {
  helligkeit = getWert();
}


/*  Der Helligkeitswert hat eine Auflösung von 10bit (Wert von 0-1023)
    Der I2C-Bus kann aber nur jeweils 1 byte (= 8 bit) senden.

    Also müssen die 10 bit in 8 bit und 2 bit aufgeteil werden und getrennt gesendet werden.
    Ansonsten hätte man statt einem Wert von 0-1023 nur einen Wert von 0-255.

    Deshalb werden die sogenannten high und low bytes separat gesendet.
*/

// helligkeit_low berechnet das Low-Byte
byte Lichtsensor::helligkeit_low() {
  return (helligkeit & 0xff );
}
// helligkeit_high berechnet das High-Byte
byte Lichtsensor::helligkeit_high() {
  return ((helligkeit >> 8) & 0xff );
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

  setLED(WEISS);
}

void Farbsensor::update() {
  setLED(ROT);
  delay(ZEIT);
  int _wert_R = 1023 - getWert();

  setLED(GRUEN);
  delay(ZEIT);
  int _wert_G = 1023 - getWert();

  setLED(BLAU);
  delay(ZEIT);
  int _wert_B = 1023 - getWert();

  setLED(WEISS);
  delay(ZEIT);
  helligkeit = 1023 - getWert();

  rot_prozent = (byte) _wert_R * 100L / ( _wert_R + _wert_G + _wert_B);
  blau_prozent = (byte) _wert_G * 100L / ( _wert_R + _wert_G + _wert_B);
  gruen_prozent = (byte) _wert_B * 100L / ( _wert_R + _wert_G + _wert_B);
}

void Farbsensor::setLED(int farbe) {
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
