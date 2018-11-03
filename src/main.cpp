/*
 
 Copyright (C) 2018 Mauricio Bustos (m@bustos.org)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

#include <Arduino.h>
#include <SPI.h>
#include <TidesControllerClient.h>

#define DEVICE_NUMBER (1)
#define MAC_LOCAL
const byte MAC[2][6] = {
  {0x04, 0xE9, 0xE5, 0x07, 0xDD, 0x7D},
  {0x04, 0xE9, 0xE5, 0x07, 0xDD, 0x65}
};
#ifdef MAC_LOCAL
const IPAddress IP[] = {
  IPAddress(169, 254, 18, 81), 
  IPAddress(169, 254, 18, 82)
};
const IPAddress SERVER(169, 254, 18, 80);
#else
const IPAddress IP[] = {
  IPAddress(192, 168, 7, 111),
  IPAddress(192, 168, 7, 112)
};
const IPAddress SERVER(192, 168, 7, 101);
#endif
const int PORT = 1999;

long readCount = 0;
long writeCount = 0;

TidesControllerClient client;

#define PIR_PIN A1
#define PIR_LED_PIN 2
#define PIR_THRESHOLD 128

bool pirState = false;
int PIRValue;
int IRSensorValue;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(PIR_LED_PIN, OUTPUT);

  client = TidesControllerClient(IP[DEVICE_NUMBER], SERVER, PORT, (byte*)MAC[DEVICE_NUMBER], DEVICE_NUMBER);

  Serial.begin(9600);
}

int readPIR() {
  return analogRead(PIR_PIN);
}

void notifyController(int value) {
  writeCount++;
  if (Serial) {
    Serial.print(writeCount);
    Serial.print(" Sent: ");
    Serial.println(value);
  }
  if (!client.connected()) {
    client.stop();
    client = TidesControllerClient(IP[DEVICE_NUMBER], SERVER, PORT, (byte*)MAC[DEVICE_NUMBER], DEVICE_NUMBER);
  }
  client.send(value);
}

void loop() {
  PIRValue = readPIR();

  bool state_change = (PIRValue > PIR_THRESHOLD && pirState == false) || (PIRValue < PIR_THRESHOLD && pirState == true);
  pirState = PIRValue > PIR_THRESHOLD;
  digitalWrite(PIR_LED_PIN, pirState);

  if (state_change) {
    notifyController(PIRValue);
  }
  client.read();
}