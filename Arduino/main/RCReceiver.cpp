/**
 * @file RCReceiver.c
 * @author Serj Babayan
 * @date Nov 6, 2016
 * @copyright Waterloo Aerial Robotics Group 2016 \n
 *   https://raw.githubusercontent.com/UWARG/WARG-Sim/master/LICENCE
 * @see https://www.arduino.cc/en/Reference/AttachInterrupt
 *
 */

#include "Arduino.h"
#include "RCReceiver.h"

static PWMData currentPWM; //actual calculated values of the signal
static PWMData currentPWMCount; //used for counting the length of the pwm signal

/* Our interrupt service routing for when we detect a change in signal
 * in any of our enabled PWM pins */
void handleInterrupt_Channel1()
{
    if(CHANNEL1_HIGH)
        currentPWMCount[0]= micros();
    else
        currentPWM[0] = micros() - currentPWMCount[0];
}

void handleInterrupt_Channel2()
{
    if(CHANNEL2_HIGH)
        currentPWMCount[1]= micros();
    else
        currentPWM[1] = micros() - currentPWMCount[1];
}

void handleInterrupt_Channel3()
{
    if(CHANNEL3_HIGH)
        currentPWMCount[2]= micros();
    else
        currentPWM[2] = micros() - currentPWMCount[2];
}

void handleInterrupt_Channel4()
{
    if(CHANNEL4_HIGH)
        currentPWMCount[3]= micros();
    else
        currentPWM[3] = micros() - currentPWMCount[3];
}

void initRCReceiver(void){
    pinMode(CHANNEL1_PIN, INPUT);
    pinMode(CHANNEL2_PIN, INPUT);
    pinMode(CHANNEL3_PIN, INPUT);
    pinMode(CHANNEL4_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(CHANNEL1_PIN),handleInterrupt_Channel1,CHANGE);
    attachInterrupt(digitalPinToInterrupt(CHANNEL2_PIN),handleInterrupt_Channel2,CHANGE);
    attachInterrupt(digitalPinToInterrupt(CHANNEL3_PIN),handleInterrupt_Channel3,CHANGE);
    attachInterrupt(digitalPinToInterrupt(CHANNEL4_PIN),handleInterrupt_Channel4,CHANGE);

    for(int i = 0;i<NUM_CHANNELS; i++){
        currentPWM[i] = 0;
        currentPWMCount[i] = 0;
    }
}

PWMData* getCurrentPWMState(void){
    return &currentPWM;
};
