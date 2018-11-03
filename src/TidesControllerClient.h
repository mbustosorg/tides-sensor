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

#ifndef TIDESCONTROLLERCLIENT_H
#define TIDESCONTROLLERCLIENT_H

#include <Ethernet.h>
#include <EthernetUDP.h>
#include <OSCMessage.h>
#include <OSCBoards.h>

enum State {UNKNOWN, DETECTED, LINKED, CONNECTED};

class TidesControllerClient {

    public:
        TidesControllerClient();
        TidesControllerClient(IPAddress self, IPAddress server, int port, byte* mac, int sensorId);
        ~TidesControllerClient();

        void stop();
        void send(int message);
        void read();
        bool connected();

    private:

        State state = UNKNOWN;

        IPAddress mServer;
        int mPort;
        byte* mMac;
        int mSensorId = 0;

        EthernetUDP mUdp;
        EthernetClient mClient;

        long mConnectionCount = 0;        
};

#endif