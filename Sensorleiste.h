#ifndef SENSORLEISTE_H
#define SENSORLEISTE_H

void init_sensorleiste(int anzahl_farbsensoren, int i2c_adresse);
void sensoren_auslesen(Farbsensor (& sensoren)[_anzahl_farbsensoren]);

struct Farbsensor {
  int helligkeit;
  byte rot_prozent;
  byte gruen_prozent;
  byte blau_prozent;
};

int _anzahl_farbsensoren;
int _i2c_adresse;

#endif
