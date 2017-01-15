#include "RCReceiver.h"
#include "Altmeter.h"

//long current_time = 0;
//PWMData* pwmState;
float altitude = 122.0;
float temperature = -32;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    //initRCReceiver();
    //current_time = micros();
    initAltmeter();
    setAltitude(123.00);
    setTemperature(temperature);
}

void loop() {
/*
    if((micros() - current_time) > 1000000){
        pwmState = getCurrentPWMState();

        current_time = micros();
        Serial.println("Throttle");
        Serial.println((*pwmState)[0]);
        Serial.println("Roll");
        Serial.println((*pwmState)[1]);
        Serial.println("Left Tail");
        Serial.println((*pwmState)[2]);
        Serial.println("Right Tail");
        Serial.println((*pwmState)[3]);
    }
    */
    delay(1000);
    altitude += 0.15;
    temperature += 0.1;
    setTemperature(temperature);
    setAltitude(altitude);
}
