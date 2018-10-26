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

TidesControllerClient::TidesControllerClient(IPAddress self, IPAddress server, int port, byte* mac) {

  while (state == State::UNKNOWN) {
    Ethernet.begin(mac, self);
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      connectionCount++;
      Serial.print("Ethernet shield was not found.  Looking for hardware...");
      Serial.println(connectionCount);
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
    if (client.connect(server, port)) {
      state = State::CONNECTED;
      Serial.println("Connected");
    } else {
      connectionCount++;
      Serial.print("Connection failed, retrying...");
      Serial.println(connectionCount);
    }
  }
}

TidesControllerClient::~TidesControllerClient() {

}

