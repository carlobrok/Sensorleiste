#include "Arduino.h"
#include <Wire.h>
#include "Sensorleiste.h"

void init_sensorleiste(int anzahl_farbsensoren, int i2c_adresse) {
  _anzahl_farbsensoren = anzahl_farbsensoren;
  _i2c_adresse = i2c_adresse;
}

void sensoren_auslesen(Farbsensor (& sensoren)[_anzahl_farbsensoren]) {
  Wire.requestFrom(_i2c_adresse, (_anzahl_farbsensoren*5) );        // Die Sensordaten vom Arduino anfordern

  unsigned long t1 = millis();
  while (Wire.available() < (_anzahl_farbsensoren*5)) {             // Kurz auf die Daten warten
    if(millis() - t1 > 200) {
      Serial.println("FEHLER! Konnte die Daten nicht empfangen");     // Wenn es länger als 200ms gedauert hat abbrechen
      break;
    }
  }

  if(Wire.available() >= _anzahl_farbsensoren*5) {
    for(int i = 0; i < _anzahl_farbsensoren && Wire.available() >= 5; i++) {
      byte high_byte = Wire.read();
      byte low_byte = Wire.read();

      sensoren[i].helligkeit = low_byte | (high_byte<<8);
      sensoren[i].rot_prozent = Wire.read();
      sensoren[i].gruen_prozent = Wire.read();
      sensoren[i].blau_prozent = Wire.read();
    }
  } else {
    Serial.println("FEHLER! Weniger Daten empfangen, als nötig");
  }
}
