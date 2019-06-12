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

#define DEVICE_TOTAL (7)
#define DEVICE_NUMBER (2) // 1-7
//#define MAC_LOCAL
const byte MAC[DEVICE_TOTAL][DEVICE_TOTAL] = {
  {0x04, 0xE9, 0xE5, 0x07, 0xDD, 0x65},
  {0x04, 0xE9, 0xE5, 0x07, 0xDD, 0x68},
  {0x04, 0xE9, 0xE5, 0x07, 0xDD, 0x7D},
  {0x04, 0xE9, 0xE5, 0x07, 0xDA, 0x00},
  {0x04, 0xE9, 0xE5, 0x07, 0xDA, 0x00},
  {0x04, 0xE9, 0xE5, 0x07, 0xDA, 0x00},
  {0x04, 0xE9, 0xE5, 0x07, 0xDA, 0x00}
};
#ifdef MAC_LOCAL
const IPAddress IP[] = {
  IPAddress(10, 0, 1, 31), 
  IPAddress(10, 0, 1, 32), 
  IPAddress(192, 168, 7, 95), 
  IPAddress(10, 0, 1, 34) 
};
const IPAddress SERVER(192, 168, 7, 91);
#else
const IPAddress IP[DEVICE_TOTAL] = {
  IPAddress(192, 168, 0, 10),
  IPAddress(192, 168, 0, 11),
  IPAddress(192, 168, 0, 12),
  IPAddress(192, 168, 0, 13),
  IPAddress(192, 168, 0, 14),
  IPAddress(192, 168, 0, 15),
  IPAddress(192, 168, 0, 16)
};
const IPAddress SERVER(192, 168, 0, 1);
#endif
const int PORT = 1999;

long readCount = 0;
long writeCount = 0;

TidesControllerClient client;

#define PIR_PIN A1
#define PIR_LED_PIN 2
#define PIR_THRESHOLD 512

bool pirState = false;
int PIRValue;
int IRSensorValue;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(PIR_LED_PIN, OUTPUT);
  //digitalWrite(PIR_LED_PIN, HIGH);

  client = TidesControllerClient(IP[DEVICE_NUMBER - 1], SERVER, PORT, (byte*)MAC[DEVICE_NUMBER - 1], DEVICE_NUMBER);

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
    client = TidesControllerClient(IP[DEVICE_NUMBER - 1], SERVER, PORT, (byte*)MAC[DEVICE_NUMBER - 1], DEVICE_NUMBER);
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