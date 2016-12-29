#include "Altmeter.h"
#include <Wire.h>

static float set_altitude;
static float set_temperature;
static int latest_reg_address; //latest stored command. Data that we send back will depend on this

//We send altititude through I2C as a 20-bit value, represented via 3 bytes
static char altitude_values[3];

//we send the temperature as a 12-bit value, in 2 bytes. The 4 msb of the last byte is the fractional component
static char temperature_values[2];

static void onAltmeterReceive(int numBytes);
static void onAltmeterRequest(void);

void initAltmeter(){
    //start I2C communication as a slave with an address that corresponds to altmeter
    Wire.begin(ALTMETER_I2C_ADDRESS);
    Wire.onRequest(onAltmeterRequest);
    Wire.onReceive(onAltmeterReceive);
}

void setAltitude(float alt){
    set_altitude = alt;
}

void setTemperature(float temp){
    set_temperature = temp;
}

/**
 * Converts the float altitude into a set of 3 bytes that our clients will interpret
 * From the documentation, the first 2 bytes (16-bits) contain the integer portion
 * of the altitude, and the 4 most significant bits of the last byte (OUT_P_LSB)
 * contains the unsigned floating point decimal. Therefore we must send it our floating
 * point altitude in the corresponding way. The altitude data sent is therefore
 * 20-bits
 **/
static void updateAltitudeValues(void){
   //cast to an int to get rid of the floating point, bit shift 8 to the right to get the 8 most significant bits
   altitude_values[0] = (((int)set_altitude) >> 8);

   //simply take the first 8 lsb of the integer altitude
   altitude_values[1] = ((int)set_altitude);

   //getting the floating point is a little bit tricky. The client expects 4 bits located as the most significant
   //bit of the last byte sent. To interprete the floating point, they will shift it 4 bits to the right, and devide
   //the resulting value by 16.0 to retrieve the floating point value. For example if we send 0b11110000, client shifts it
   //to 0b00001111, and devides by 16. Therefore we must multiply our floating point by 16, then shift 4 to the left.
   altitude_values[2] = (int)(((set_altitude - (int)set_altitude))*16); //get the floating point of our number and multiply by 16, cast to int
   altitude_values[2] <<= 4; //bitshift so the last 4 msb contain the floating point
}

/**
 * Converts the floating point temperature (in C)
 */
static void updateTemperatureValues(void){
  
  temperature_values[0] = (int)set_temperature;

  //subtract 1 since the client is expecting the 2's complement representation for negative numbers, and thus
  //will add one when parsing
  if(set_temperature < 0){
    temperature_values[0] --;
  }
  temperature_values[1] = (int)((set_temperature - (int)set_temperature)*16);
  temperature_values[1] <<= 4;
}

/**
 * When we receive a command for the altmeter
 * Note this function must return void and take in an integer, as described in the
 * arduino specifications
 **/
static void onAltmeterReceive(int numBytes){
  //just read the register address received, and discard any values that we may have received otherwise
  latest_reg_address = Wire.read();

  //clear the buffer since we don't care about the command values we received
  while(Wire.available()){
      Wire.read();
  }
}

/**
 * When we receive a request for data for the altmeter
 * Note the function return type and parameters must both be void
 **/
static void onAltmeterRequest(void){
    switch(latest_reg_address){
        case STATUS:
            //Only the 2nd bit needs to be set to 1. This lets the clients know that new altitude/pressure data is available
            //In our case since we're simulating it it will always be available, so we'll set it to 1
            //Read the datasheet for the STATUS register for more info
            Wire.write(0b00000010);
            break;
        case OUT_P_MSB: //if the client is requesting altitude/pressure data
            updateAltitudeValues();
            Wire.write(altitude_values[0]);
            Wire.write(altitude_values[1]);
            Wire.write(altitude_values[2]);
            break;
        case OUT_T_MSB: //if the client is requesting temperature
            updateTemperatureValues();
            Wire.write(temperature_values[0]);
            Wire.write(temperature_values[1]); //write 0 for now
            break;
        default: //just send something random for anything else the client wants
            Wire.write(0b00000000);
            break;
    }
}
