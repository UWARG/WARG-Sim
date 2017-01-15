/**
 * @file IMU.hpp
 * @author Serj Babayan
 * @date Jan 14, 2017
 * @brief IMU simulation with the Arduino MEGA
 * @copyright Waterloo Aerial Robotics Group 2016 \n
 *   https://raw.githubusercontent.com/UWARG/WARG-Sim/master/LICENCE
 *
 * @notes This will setup the Arduino Mega's SPI pins in slave mode to simulate the IMU. Unlike
 * the GPS, this uses Arduino's hardware for SPI communication with the PicPilot
 **/

/**
 * This pin is required for parsing SPI commands. Its input will have to be in parallel with the slave select pin
 * on the arduino mega (pin)
*/
#define SLAVE_SELECT_PIN 51

/*
 * VN-100 Registers that are used within the picpilot. The rest can be found in
 * the VN100.h file in the Picpilot code base
 */
#define     VN100_REG_MODEL     1
#define     VN100_REG_YPR       8
#define     VN100_REG_GYR       19

/* Relevant command IDs that are used within the picpilot. The rest can be found in VN100.h*/
#define VN100_CMD_READ_REGISTER 0x01

/**
 * Initializes SPI as slave and other parameters required to simulate the IMU
 */
void initializeIMU(void);

/**
 * Set the IMU rates to be sent back to the picpilot, in rad/s
 */
void setIMURates(float roll_rate, float pitch_rate, float yaw_rate);

/**
 * Set the IMU angles in degrees to be sent back to the picpilot
 */
void setIMUAngles(float roll, float pitch, float yaw);
