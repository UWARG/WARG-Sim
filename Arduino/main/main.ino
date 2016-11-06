#include "RCReceiver.h"

long current_time = 0;
PWMData* pwmState;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
	
	initRCReceiver();
  current_time = micros();
}

void loop() {
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
}
