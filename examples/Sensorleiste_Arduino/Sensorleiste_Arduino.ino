#include <Wire.h>
#include "Sensor.h"

/*
  Beispiel für eine Sensorleiste mit 4 Farbsensoren. Die ausgelesenen Daten werden per I2C-Bus gesendet.
  Wenn DEBUG definiert ist, werden die Werte auch auf der Seriellen Schnittstelle ausgegeben und können auf dem PC
  angezeigt werden.

  Autor: Carlo Brokering, 2019
*/

#define ADRESSE 0x08      // Die I2C-Adresse
#define ANZAHL_SENSOREN 4       // Wie viele Sensoren sich an der Sensorleiste befinden

// Um Ressourcen zu sparen diese Zeile vor dem Übertragen auskommentieren.
// So werden die Werte nicht mehr per Serieller Schnittstelle ausgegeben.
#define DEBUG


Farbsensor sensorleiste [ANZAHL_SENSOREN];      // Ein Array mit den Farbsensoren
int sensorwerte[ANZAHL_SENSOREN*4];     // Ein Array für die Werte der Farbsensoren



void setup()
{
  Wire.begin(ADRESSE);                // Der Arduino tritt dem I2C-Bus mit der ADRESSE bei
  Wire.onRequest(requestEvent);       // Wenn der Arduino einen request Befehl erhält wird die Funktion requestEvent ausgeführt

  // Die Pins der Farbsensoren festlegen
  sensorleiste[0] = Farbsensor(A0, 2, 3, 4);
  sensorleiste[1] = Farbsensor(A1, 5, 6, 7);
  sensorleiste[2] = Farbsensor(A2, 8, 9, 10);
  sensorleiste[3] = Farbsensor(A3, 11, 12, 13);

#ifdef DEBUG                       // Die Serielle Schnittstelle wird nur initialisiert, wenn DEBUG definiert ist
  Serial.begin(115200);                       // Serieller Monitor an, um am Computer die Werte zu sehen.
#endif
}

// Die Endlosschleife
void loop()
{
  for(int i = 0; i < ANZAHL_SENSOREN; i++)      // Alle Sensorwerte auslesen
  {
    sensorleiste[i].getWerte(sensorwerte[i*4], sensorwerte[i*4+1], sensorwerte[i*4+2], sensorwerte[i*4+3]);
  }


#ifdef DEBUG                // Alle Werte über die Serielle Schnittstelle ausgegeben
  for (int i=0; i<16; i++)
  {
    if (i % 4 == 0 || i == 0) {
      Serial.print("S");
      Serial.print(16/i + 1);
      Serial.print(" ");
    }
    Serial.print(sensorwerte[i]);
    if (i%4 == 3) Serial.print("\t");
      else        Serial.print(" ");
  }
  Serial.println();
#endif
}

// Diese Funktion wird aufgerufen, wenn ein anderes Gerät von diesem Arduino Daten anfordert
// (requestFrom: https://www.arduino.cc/en/Reference/WireRequestFrom)
void requestEvent() {
  for(int i = 0; i<16;i++)
  {
    /*  Jeder 4. und der erste Wert hat eine Auflösung von 10bit (Wert von 0-1023)
        Der I2C-Bus kann aber nur jeweils 1 byte (= 8 bit) senden.

        Also müssen die 10 bit in 8 bit und 2 bit aufgeteil werden und getrennt gesendet werden.
        Ansonsten hätte man statt einem Wert von 0-1023 nur einen Wert von 0-255.
    */
    if(i%4 == 0 || i == 0) {
      byte high_byte = ((sensorwerte[i] >> 8) & 0xff );
      byte low_byte = (sensorwerte[i] & 0xff );
      Wire.write(high_byte);
      Wire.write(low_byte);
    }

    // Die 3 anderen Werte sind Prozentwerte von 0-100 und können somit so übertragen werden.
    else {
      Wire.write((byte)sensorwerte[i]);
    }
  }
}
