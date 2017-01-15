#ifndef _ARDUINO_COMMS_HH_
#define _ARDUINO_COMMS_HH_

#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <sstream>

#pragma pack(1)

using namespace std;
using namespace LibSerial;

typedef struct _DataPayload{
    double longitude, latitude;
    float rollRate, pitchRate, yawRate;
    float yaw, pitch, roll;
    float altitude;
} DataPayload;

typedef struct _ArduinoPacket{
    char commandID:4;
    char crc:4;
    char length;
    DataPayload data;
} ArduinoPacket;


class Arduino{
    private:
        SerialStream serial_port;
        string portName;
        char crcRemainder(char* input, char lenInput);
        char crcByteMessage(char const message);
    public:
        Arduino(string input);
        bool DataAvailable();
        string GetData();
        
        ArduinoPacket* loadPacket(float rRate, float pRate, float yRate, float
y, float p, float r, float altitude, double longitude, double latitude);
        void sendData(ArduinoPacket* ap);
        void sendData(string input)
        {
            serial_port << input;  
        }
};
#endif
