#include "ArduinoComms.hh"

char Arduino::crcRemainder(char* input, char lenInput){
    int rem = 0;
    char message = input[0];
    for (int i = 0; i < lenInput; i++){
        rem = Arduino::crcByteMessage(message);
        if (i+1 < lenInput){
            message = input[i+1];
            message ^= rem;
        }
    } 
    return (rem >> 4);
}

char Arduino::crcByteMessage(char const message){
    char remainder; 
    const char POLYNOMIAL = 0b10110000;
    /*
     * Initially, the dividend is the remainder.
     */
    remainder = message;
    /*
     * For each bit position in the message....
     */
    for (char bit = 8; bit > 0; --bit)
    {
    /*
     * If the uppermost bit is a 1...
     */
        if (remainder & 0x80){
        /*
         * XOR the previous remainder with the divisor.
         */
         remainder ^= POLYNOMIAL;
        }
         
        /*
         * Shift the next bit of the message into the remainder.         
         */
        remainder = (remainder << 1);
    }
        /*
         * Return only the remainder.
         */
    return remainder;
}

Arduino::Arduino(string input){
    portName = input;
    
    serial_port.Open( portName ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] " << "Error: Could not open serial port." << std::endl ;
        exit(1) ;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        exit(1) ;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the number of stop bits." << std::endl ;
        exit(1) ;
    }
    //
    // Turn off hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not use hardware flow control." << std::endl ;
        exit(1) ;
    }

   /*  // Wait for some data to be available at the serial port.
    while( serial_port.rdbuf()->in_avail() == 0 )
    {
        usleep(100) ;
    }*/
}

bool Arduino::DataAvailable()
{
    if (serial_port.rdbuf()->in_avail() > 0)
        return 1;
    else
        return 0;
}

string Arduino::GetData()
{
    string output;
    output = "";

    bool packetReceived = 0;
    bool packetStarted = 0;
    while (!packetReceived){
        if (serial_port.rdbuf()->in_avail() == 1)
        {
            char next_byte;
            serial_port.get(next_byte);
            if (!packetStarted)
            {
                if (next_byte == '(')
                    packetStarted = 1;
            }

            else
            {
                if (next_byte == ')')
                    packetReceived = 1;
                else
                    output += next_byte;
            }
        }
    }
    
    return output;
}

ArduinoPacket* Arduino::loadPacket(float rRate, float pRate, float yRate, float
y, float p, float r, float altitude, double longitude, double latitude){
    ArduinoPacket* ap = new ArduinoPacket();
    ap->data.rollRate = rRate;
    ap->data.pitchRate = pRate;
    ap->data.yawRate = yRate;
    ap->data.yaw = y;
    ap->data.pitch = p;
    ap->data.roll = r;
    ap->data.altitude = altitude;
    ap->data.longitude = longitude;
    ap->data.latitude = latitude;
    
    cout << ap->data.pitch << endl;

    //Set cmdID to 0, may be used for future implementations
    ap->commandID = 0;
    ap->length = sizeof(ap->data) & 0xFF;
    ap->crc = crcRemainder((char*)(&ap->data),ap->length);  
    cout << ap->data.pitch << endl;
    return ap;
}

void Arduino::sendData(ArduinoPacket* ap){
    cout << ap->data.pitch << endl; 
    
    int pLen = sizeof(*ap);
    cout << pLen << endl;
    char sendChar;
    for (int i = 0; i < pLen; i++){
        sendChar = *((char*)(ap + i));
        serial_port << sendChar;
        cout << " 0b";
        for (int j = 0; j < 8; j++){
            cout << ((sendChar >> j) & 1);
        }
        //cout << printf("%c",sendChar);
    }
    cout << endl;
    delete ap;
}

