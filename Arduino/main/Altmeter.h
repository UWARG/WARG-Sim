/**
 * @file Altmeter.h
 * @author Serj Babayan
 * @date Dec 28, 2016
 * @brief Altmeter sensor simulation using the Arduino Mega
 * @copyright Waterloo Aerial Robotics Group 2016 \n
 *   https://raw.githubusercontent.com/UWARG/WARG-Sim/master/LICENCE
 *
 * @see https://www.sparkfun.com/products/11084 for documentation on the sensor, including
 * its data sheet and existing reference arduino library that this was based off of
 *
 * @notes
 * Because a single arduino can enter an I2C bus as either a master or a slave with a specified master,
 * it may not be possible to simulate multiple I2C sensors with a single arduino. For now this is fine
 * as the only I2C sensor used on the picpilot is the altmeter.
 *
 * Also please note that the sensor being simulated will ALWAYS be in ALTITUDE_MODE simply
 * because we don't really care about BAROMETER mode in our case (we want heights not pressure). If you're
 * confused as to what the two modes do read the data sheet for the sensor.
 **/

/** Address predefined as listed in the datasheet **/
#define ALTMETER_I2C_ADDRESS 0x60

/**
 * Define various register addresses that correspond to the altitude sensor we're using
 * The names correspond to the register values found in the data sheet. There are far more
 * but these are the ones we actually care about
 **/
#define STATUS     0x00
#define OUT_P_MSB  0x01
#define OUT_T_MSB  0x04

/**
 * Sets up i2c communication as a slave on the bus
 */
void initAltmeter(void);

/**
 * Set the altitude that the arduino mega will send to the picpilot through
 * I2C. In metres
 **/
void setAltitude(float alt);

/**
 * Sets the temperature that the arduino mega will send back. Temperature is currently
 * not used within the picpilot, however it may be used in some other arduino examples
 * for the altitude sensor. In C degrees. Note the highest temperature that can be sent is
 * limited by the 12-bit value that the sensor is expected to send back, therefore it must be between
 * -127 and 128
 **/
void setTemperature(float temp);
