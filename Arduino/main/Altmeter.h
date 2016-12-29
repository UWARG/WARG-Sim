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
 * The names correspond to the register values found in the data sheet
 **/
#define STATUS     0x00
#define OUT_P_MSB  0x01
#define OUT_P_CSB  0x02
#define OUT_P_LSB  0x03
#define OUT_T_MSB  0x04
#define OUT_T_LSB  0x05
#define DR_STATUS  0x06
#define OUT_P_DELTA_MSB  0x07
#define OUT_P_DELTA_CSB  0x08
#define OUT_P_DELTA_LSB  0x09
#define OUT_T_DELTA_MSB  0x0A
#define OUT_T_DELTA_LSB  0x0B
#define WHO_AM_I   0x0C
#define F_STATUS   0x0D
#define F_DATA     0x0E
#define F_SETUP    0x0F
#define TIME_DLY   0x10
#define SYSMOD     0x11
#define INT_SOURCE 0x12
#define PT_DATA_CFG 0x13
#define BAR_IN_MSB 0x14
#define BAR_IN_LSB 0x15
#define P_TGT_MSB  0x16
#define P_TGT_LSB  0x17
#define T_TGT      0x18
#define P_WND_MSB  0x19
#define P_WND_LSB  0x1A
#define T_WND      0x1B
#define P_MIN_MSB  0x1C
#define P_MIN_CSB  0x1D
#define P_MIN_LSB  0x1E
#define T_MIN_MSB  0x1F
#define T_MIN_LSB  0x20
#define P_MAX_MSB  0x21
#define P_MAX_CSB  0x22
#define P_MAX_LSB  0x23
#define T_MAX_MSB  0x24
#define T_MAX_LSB  0x25
#define CTRL_REG1  0x26
#define CTRL_REG2  0x27
#define CTRL_REG3  0x28
#define CTRL_REG4  0x29
#define CTRL_REG5  0x2A
#define OFF_P      0x2B
#define OFF_T      0x2C
#define OFF_H      0x2D

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
