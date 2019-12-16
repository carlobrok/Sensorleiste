#include <Wire.h>
#include "Sensor.h"

/*
  Beispiel für eine Sensorleiste mit 4 Farbsensoren. Die ausgelesenen Daten werden per I2C-Bus gesendet.
  Wenn DEBUG definiert ist, werden die Werte auch auf der Seriellen Schnittstelle ausgegeben und können auf dem PC
  angezeigt werden.

  Autor: Carlo Brokering, 2019
*/

#define CMD_FARBSENSOREN 1
#define CMD_LICHTSENSOREN 2
int command = 0;


#define ADRESSE 0x08      // Die I2C-Adresse
#define ANZAHL_FARBSENSOREN 4       // Wie viele Farbsensoren sich an der Sensorleiste befinden
#define ANZAHL_LICHTSENSOREN 2      // Wie viele Lichtsensoren sich an der Sensorleiste befinden

// Um Ressourcen zu sparen diese Zeile vor dem Übertragen auskommentieren.
// So werden die Werte nicht mehr per Serieller Schnittstelle ausgegeben.
#define DEBUG

Farbsensor farbsensoren [ANZAHL_FARBSENSOREN];      // Ein Array mit den Farbsensoren
Lichtsensor lichtsensoren [ANZAHL_LICHTSENSOREN];     // Ein Array mit den Lichtsensoren

void setup()
{
  Wire.begin(ADRESSE);                // Der Arduino tritt dem I2C-Bus mit der ADRESSE bei
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);       // Wenn der Arduino einen request Befehl erhält wird die Funktion requestEvent ausgeführt


  // Die Pins der Farbsensoren festlegen
  farbsensoren[0] = Farbsensor(A0, 2, 3, 4);
  farbsensoren[1] = Farbsensor(A1, 5, 6, 7);
  farbsensoren[2] = Farbsensor(A2, 8, 9, 10);
  farbsensoren[3] = Farbsensor(A3, 11, 12, 13);

  // Die Pins der Lichtsensoren festlegen
  lichtsensoren[0] = Lichtsensor(A6);
  lichtsensoren[1] = Lichtsensor(A7);

  pinMode(LED_BUILTIN, OUTPUT);
#ifdef DEBUG                       // Die Serielle Schnittstelle wird nur initialisiert, wenn DEBUG definiert ist
  Serial.begin(115200);                       // Serieller Monitor an, um am Computer die Werte zu sehen.
#endif
}

// Die Endlosschleife
void loop()
{
  // Alle Sensorwerte updaten
  for(int i = 0; i < ANZAHL_FARBSENSOREN; i++) {
    farbsensoren[i].update();
  }

  for(int i = 0; i < ANZAHL_LICHTSENSOREN; i++) {
    lichtsensoren[i].update();
  }


// Alle Werte über die Serielle Schnittstelle ausgegeben
#ifdef DEBUG
  for (int i=0; i<ANZAHL_FARBSENSOREN; i++)
  {
    Serial.print("F");
    Serial.print(i);
    Serial.print(": ");

    Serial.print(farbsensoren[i].helligkeit);
    Serial.print(" ");
    Serial.print(farbsensoren[i].prozent_rot);
    Serial.print(" ");
    Serial.print(farbsensoren[i].prozent_gruen);
    Serial.print(" ");
    Serial.print(farbsensoren[i].prozent_blau);
    Serial.print("\t");
  }
  for(int i = 0; i < ANZAHL_LICHTSENSOREN; i++) {
    Serial.print("L");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(lichtsensoren[i].helligkeit);
    Serial.print("\t");
  }
  Serial.println();
#endif
}



void receiveEvent() {

  digitalWrite(LED_BUILTIN, HIGH);

  command = Wire.read();

#ifdef DEBUG
  Serial.print(command);
  if (command == CMD_FARBSENSOREN) {
    Serial.println(" > Farbsensoren Daten senden");
  } else if (command == CMD_LICHTSENSOREN) {
    Serial.println(" > Lichtsensoren Daten senden");
  }
#endif

  digitalWrite(LED_BUILTIN, LOW);
}



// Diese Funktion wird aufgerufen, wenn ein anderes Gerät von diesem Arduino Daten anfordert
// (requestFrom: https://www.arduino.cc/en/Reference/WireRequestFrom)
void requestEvent() {
  if(command == CMD_FARBSENSOREN) {
    for(int i = 0; i < ANZAHL_FARBSENSOREN; i++) {
      /*  Der Helligkeitswert hat eine Auflösung von 10bit (Wert von 0-1023)
          Der I2C-Bus kann aber nur jeweils 1 byte (= 8 bit) senden.

          Also müssen die 10 bit in 8 bit und 2 bit aufgeteil werden und getrennt gesendet werden.
          Ansonsten hätte man statt einem Wert von 0-1023 nur einen Wert von 0-255.

          Deshalb werden die sogenannten high und low bytes separat gesendet.
      */
      Wire.write(farbsensoren[i].helligkeit_high);
      Wire.write(farbsensoren[i].helligkeit_low);

      // Die 3 anderen Werte sind Prozentwerte von 0-100 und können somit übertragen werden, ohne sie zu verändern.
      Wire.write(farbsensoren[i].prozent_rot);
      Wire.write(farbsensoren[i].prozent_gruen);
      Wire.write(farbsensoren[i].prozent_blau);
    }
  } else if (command == CMD_LICHTSENSOREN) {
    for(int i = 0; i < ANZAHL_LICHTSENSOREN; i++) {
      /*  Der Helligkeitswert hat eine Auflösung von 10bit (Wert von 0-1023)
          Der I2C-Bus kann aber nur jeweils 1 byte (= 8 bit) senden.

          Also müssen die 10 bit in 8 bit und 2 bit aufgeteil werden und getrennt gesendet werden.
          Ansonsten hätte man statt einem Wert von 0-1023 nur einen Wert von 0-255.

          Deshalb werden die sogenannten high und low bytes separat gesendet.
      */
      Wire.write(lichtsensoren[i].helligkeit_high);
      Wire.write(lichtsensoren[i].helligkeit_low);
    }
  }
}
