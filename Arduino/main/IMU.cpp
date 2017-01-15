/**
   @file IMU.cpp
   @author Serj Babayan
   @date Jan 14, 2017
   @copyright Waterloo Aerial Robotics Group 2016 \n
     https://raw.githubusercontent.com/UWARG/WARG-Sim/master/LICENCE
   @see https://github.com/GreyGnome/PinChangeInt
   @see http://playground.arduino.cc/Code/QueueArray
 **/

#include "IMU.hpp"
#include <SPI.h>
#include <QueueArray.h>
#include <PinChangeInt.h>

/**
   Stores the yaw, pitch, and roll rates we'll send back to the picpilot. Order is: 1) roll rate 2) pitch rate 3) yaw rate
*/
static char attitude_rates[12];

/**
   Same thing as rates with the same size, only these are the angles we'll send back. Order is: 1) roll 2) pitch 3) yaw
*/
static char attitude_angles[12];

static const char* model_name = "VN-100T-SMD"; //also 12 bytes in length. Picpilot expects this value to be sent back

volatile static char write_index; //used to keep track of the current byte of the above variables we're sending to the picpilot

/**
   Because IMU communication is done with 32-bit words, we'll use this as a temporary buffer
   until we get the full word. When we get the full word from the spi communication, we'll transfer it
   over to the spi_buffer for later processing
*/
static char byte_buffer[4];
static char byte_index;

/**
   Store the register id that the picpilot wants to read from so we send the correct data back
*/
volatile static char register_id;

/**
   This enum is used to keep track of the current state of the spi bus. If the state is
   READING_COMMAND, then the next bytes we receive through SPI will be a command from the
   picpilot that we will later respond to. If its writing data, then that means we're supposed
   to be sending data back to the picpilot, and we shouldnt interpret the commands the picpilot
   is sending us since they're junk
*/
static enum SPI_STATUS {
  READING_COMMAND,
  WRITING_DATA
} spi_status;

/**
   Used to store all the 32-bit words we receive from the spi bus
*/
volatile static QueueArray <unsigned long> spi_buffer;

static void spiTransactionDone(void);
static void writeFloatToCharArray(float num1, float num2, float num3, char* writeTo);
static void setSPIDataOut(char register_id, char write_index);

void initializeIMU(void) {
  SPCR |= bit(SPE); //spi enable

  // have to send on master in, *slave out*. Pins MOSI, SS, and CLK should be correctly configured by default
  pinMode(MISO, OUTPUT);

  byte_index = 0;
  write_index = 0;
  spi_status = READING_COMMAND; //next bytes will indicate the command

  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode (SPI_MODE2);

  //Attach an interrupt handler for when the SPI transaction ends. Because slave select is active high, need to do on rising
  attachPinChangeInterrupt(SS, spiTransactionDone, RISING);

  // turn on spi interrupts
  SPCR |= _BV(SPIE);
}

void setIMURates(float roll_rate, float pitch_rate, float yaw_rate) {
  writeFloatToCharArray(roll_rate, pitch_rate, yaw_rate, attitude_rates);
}

void setIMUAngles(float roll, float pitch, float yaw) {
  writeFloatToCharArray(roll, pitch, yaw, attitude_angles);
}

/**
   Our Interrupt service routine for when we receive a byte from the SPI bus.
   We will receive as well as send over the data in this routine
*/
ISR (SPI_STC_vect)
{
  byte_buffer[byte_index] = SPDR; // grab byte from SPI Data Register
  byte_index++;

  if (byte_index == 4) { //if we've reached a word
    spi_buffer.push((unsigned long)byte_buffer); //enqueue the byte so we'll read from it later
    byte_index = 0; //get ready for the next word
  }

  //logic for sending out bytes
  if (spi_status == WRITING_DATA) { //if we're in the middle of sending data
    setSPIDataOut(register_id, write_index);
    write_index++;
  } else {
    SPDR = 0;
  }
}

/**
   This function is called when the slave select line changes from low to high,
   which is when the master indicated that the SPI transaction has ended. Here we can process
   the set data we just received, and determine the next steps
*/
static void spiTransactionDone(void) {
  if (spi_status == READING_COMMAND) { //if the picpilot just finished sending the request header
    unsigned long command;
    unsigned char command_id;

    command = spi_buffer.pop();

    //data is sent in little-endian so command_id should come first ,then register id
    command_id = command & 0xFF;

    if (command_id == VN100_CMD_READ_REGISTER) { //if its a register read command (means we have to send data back)
      register_id = command & (0xFF << 8); //register id will be the second byte from the last bit
      spi_status = WRITING_DATA;
      write_index = 0;
    }
  } else if (spi_status == WRITING_DATA) { //if we just finished writing data over to the picpilot
    spi_status = READING_COMMAND; //get ready to receive the next commands
  }
  //clear out any data in the buffer as its junk / we dont care about it
  while (!spi_buffer.isEmpty()) {
    spi_buffer.pop();
  }
}

/**
   Sets the next byte of spi data to send out based on the value of the register id
*/
static void setSPIDataOut(char register_id, char write_index) {
  switch (register_id) {
    case VN100_REG_MODEL:
      SPDR = model_name[write_index];
      break;
    case VN100_REG_YPR:
      SPDR = attitude_angles[write_index];
      break;
    case VN100_REG_GYR:
      SPDR = attitude_rates[write_index];
      break;
    default:
      SPDR = 0;
      break;
  }
}

/**
   A helper function that writes 3 floats to a character array of size 12.
   Note we dont need to manipulate the floats.
   Just store in our character array so we can just send it over to the picpilot over spi
*/
static void writeFloatToCharArray(float num1, float num2, float num3, char* writeTo) {
  char* x;

  for (int i = 0; i < 12; i++) {
    if (i = 0) {
      x = (char*)&num1;
    } else if (i = 4) {
      x = (char*)&num2;
    } else if (i = 8) {
      x = (char*)&num3;
    }
    writeTo[i] = x[i % 4];
  }
}
