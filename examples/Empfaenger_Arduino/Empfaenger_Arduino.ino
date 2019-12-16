#include <Wire.h>
#include "Sensorleiste.h"

Farbsensor sensoren[4];

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  init_sensorleiste(0x08);
  Serial.begin(115200);     // Der serielle Monitor wird aktiviert.
}

void loop()
{
  farbsensoren_auslesen(sensoren);

  for(int i = 0; i < 4; i++) {
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(sensoren[i].helligkeit);
    Serial.print(" ");
    Serial.print(sensoren[i].rot_prozent);
    Serial.print(" ");
    Serial.print(sensoren[i].gruen_prozent);
    Serial.print(" ");
    Serial.print(sensoren[i].blau_prozent);
    Serial.print("\t");
  }
}
