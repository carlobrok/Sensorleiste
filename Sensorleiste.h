#ifndef SENSORLEISTE_H
#define SENSORLEISTE_H

#include "Arduino.h"
#include <Wire.h>

#define CMD_FARBSENSOREN 1
#define CMD_LICHTSENSOREN 2

int _i2c_adresse;

struct Farbsensor {
  int helligkeit;
  byte rot_prozent;
  byte gruen_prozent;
  byte blau_prozent;
};

void init_sensorleiste(int i2c_adresse) {
  _i2c_adresse = i2c_adresse;
}

template <int N>
void farbsensoren_auslesen(Farbsensor (& sensoren)[N]) {
  // Dem anderen Arduino mitteilen, dass er gleich die Werte der Farbsensoren senden soll
  Wire.beginTransmission(_i2c_adresse);
  Wire.write(CMD_FARBSENSOREN);
  Wire.endTransmission();

  int bytes_empfangen = Wire.requestFrom(_i2c_adresse, (N*5) );        // Die Sensordaten vom Arduino anfordern

  if(bytes_empfangen == N*5) {
    for(int i = 0; i < N && Wire.available() >= 5; i++) {
      byte high_byte = Wire.read();     // Zuerst kommt das High-Byte
      byte low_byte = Wire.read();      // Danach das Low-Byte

      sensoren[i].helligkeit = low_byte | (high_byte<<8);     // Den 10bit Wert der Helligkeit wieder zusammensetzen
      sensoren[i].rot_prozent = Wire.read();                  // Als drittes kommt der Rotwert
      sensoren[i].gruen_prozent = Wire.read();                // Als viertes der Grünwert
      sensoren[i].blau_prozent = Wire.read();                 // Als fünftes der Blauwert
    }
  } else {
    Serial.println("FEHLER! Nicht die richtige Datenmenge empfangen");
  }
}

template <int N>
void lichtsensoren_auslesen(int (& lichtsensor_werte)[N]) {
  // Dem anderen Arduino mitteilen, dass er gleich die Werte der Lichtsensoren senden soll
  Wire.beginTransmission(_i2c_adresse);
  Wire.write(CMD_LICHTSENSOREN);
  Wire.endTransmission();

  int bytes_empfangen = Wire.requestFrom(_i2c_adresse, N);

  if(bytes_empfangen == N) {
    for(int i = 0; i < N; i++) {
      lichtsensor_werte[i] = Wire.read();
    }
  } else {
    Serial.println("FEHLER! Nicht die richtige Datenmenge empfangen!");
  }
}

#endif
