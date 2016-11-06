/**
 * @file RCReceiver.h
 * @author Serj Babayan
 * @date Nov 6, 2016
 * @brief Arduino Mega header file for receiving PWM inputs from 
 * predefined ports. Used to get values from RC controller. Note that
 * only an Arduino Mega is useful for these, as it has 6 pins that support
 * hardware interrupts, while the Uno only has 2.
 * @copyright Waterloo Aerial Robotics Group 2016 \n
 *   https://raw.githubusercontent.com/UWARG/WARG-Sim/master/LICENCE
 * @see https://www.arduino.cc/en/Reference/AttachInterrupt
 * 
 */
 
#ifndef RCRECEIVER
#define RCRECEIVER
 
 //If you change this, add a definition to the ones below and create
 //another interrupt service routine for the new channel in the implementation
 #define NUM_CHANNELS 4
 
 /* We use interrupts for reading pwm values instead of the arduino pwm 
  * library since with that our system would hang waiting for the pwm 
  * value to drop low. Since only pins 2, 3, 18, 19, 20, 21 are 
  * available for interrupt use on the Arduino Mega, we select the 
  * following values for the rc inputs. We choose 18 and 19 in place of
  * 20 and 21 since these are also SDA AND SDC pins that we'll use for
  * I2C later. */
 #define CHANNEL1_PIN 2 //only digital pin
 #define CHANNEL2_PIN 3 //only digital pin
 #define CHANNEL3_PIN 18 //also TX1
 #define CHANNEL4_PIN 19 //also RX1
 
 
 /* Our macros for retrieving pin values from the register super quickly.
  * Note that thse are only valid for the Mega, and if you change any of 
  * the values above ^^, make sure to change these too! Please see this
  * https://www.arduino.cc/en/Reference/PortManipulation for how to get
  * port values using registers, and refer to the pin out of the mega 
  * found here: https://www.arduino.cc/en/uploads/Hacking/PinMap2560big.png
  */
#define CHANNEL1_HIGH (PINE & 0b00010000)
#define CHANNEL2_HIGH (PINE & 0b00100000)
#define CHANNEL3_HIGH (PIND & 0b00001000)
#define CHANNEL4_HIGH (PIND & 0b00000100)

typedef volatile long PWMData[NUM_CHANNELS]; //we only have 4 channels thus far

void initRCReceiver(void);

/**
 * @return A pointer that holds the current received PWM state
 */
PWMData* getCurrentPWMState(void);

#endif
