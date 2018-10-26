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

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
//169.254.35.211
IPAddress ip(169, 254, 131, 109);
IPAddress server(169, 254, 131, 108);
//IPAddress ip(192, 168, 1, 177);
//IPAddress server(1, 1, 1, 1);
int port = 1999;

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

  client = TidesControllerClient(ip, server, port, mac);

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
  if (!client.client.connected()) {
    client.client.stop();
    client = TidesControllerClient(ip, server, port, mac);
  }
  client.client.print(value);
}

void loop() {
  PIRValue = readPIR();

  bool state_change = (PIRValue > PIR_THRESHOLD && pirState == false) || (PIRValue < PIR_THRESHOLD && pirState == true);
  pirState = PIRValue > PIR_THRESHOLD;
  digitalWrite(PIR_LED_PIN, pirState);

  if (state_change) {
    notifyController(PIRValue);
  }
  if (client.client.available()) {
    char c = client.client.read();
    readCount++;
    Serial.print(readCount);
    Serial.print(" Received: ");
    Serial.println(c);
  }
}