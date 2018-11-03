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

#include "TidesControllerClient.h"

TidesControllerClient::TidesControllerClient() {
}

TidesControllerClient::TidesControllerClient(IPAddress self, IPAddress server, int port, byte* mac, int sensorId) {

  mServer = server;
  mPort = port;
  mMac = mac;
  mSensorId = sensorId;

  while (state == State::UNKNOWN) {
    Ethernet.begin(mac, self);
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      mConnectionCount++;
      Serial.print("Ethernet shield was not found.  Looking for hardware...");
      Serial.println(mConnectionCount);
      delay(1000);
    } else {
      state = State::DETECTED;
      Serial.println("Hardware Detected...");
    } 
  }
  while (state == State::DETECTED) {
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected...");
      delay(1000);
    } else {
      state = State::LINKED;
      Serial.println("Linked...");
    }
  }
  delay(1000);
  Serial.println("Connecting...");
  while (state == State::LINKED) {
    if (mUdp.begin(port)) {
      state = State::CONNECTED;
      Serial.println("Connected");
    } else {
      mConnectionCount++;
      Serial.print("Connection failed, retrying...");
      Serial.println(mConnectionCount);
    }
  }
}

TidesControllerClient::~TidesControllerClient() {

}

void TidesControllerClient::stop() {
  mUdp.stop();
}

void TidesControllerClient::send(int message) {

  OSCMessage msg("/LEDPlay/player/foregroundRunIndex");
  msg.add(mSensorId);
  msg.add(message);
  
  mUdp.beginPacket(mServer, mPort);
  msg.send(mUdp); 
  mUdp.endPacket(); 
  msg.empty(); 
}

void TidesControllerClient::read() {
  if (mUdp.available()) {
    char c = mUdp.read();
    Serial.print("Received: ");
    Serial.println(c);
  }
}

bool TidesControllerClient::connected() {
  return true; //mUdp.connected();
}