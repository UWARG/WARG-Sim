
//PWM Setup for RC Controller
volatile long rc_throttle = 0;
volatile long rc_throttle_count = 0;

volatile long rc_roll = 0;
volatile long rc_roll_count = 0;

volatile long rc_rtail = 0;
volatile long rc_rtail_count = 0;

volatile long rc_ltail = 0;
volatile long rc_ltail_count = 0;

long current_time = 0;

/*We use interrupts for reading pwm values instead of the arduino pwm library
since with that our system would hang waiting for the pwm value to drop low.
Since only pins 2, 3, 18, 19, 20, 21 are available for interrupt use on the
Arduino Mega, we select the following values for the rc inputs */
#define THROTTLE_PIN 2
#define ROLL_PIN 3
#define RTAIL_PIN 18
#define LTAIL_PIN 19

// PIN 2
#define THROTTLE_PIN_ON (PINE & 0b00010000)

// PIN 3
#define ROLL_PIN_ON (PINE & 0b00100000)

// PIN 18
#define  RTAIL_PIN_ON (PIND & 0b00001000)

// PIN 19
#define LTAIL_PIN_ON (PIND & 0b00000100)

void handleInterrupt_Throttle()  
{  
  if(THROTTLE_PIN_ON)
    rc_throttle_count= micros();
  else  
    rc_throttle= micros() - rc_throttle_count;
}

void handleInterrupt_Roll()  
{  
  if(ROLL_PIN_ON)  
    rc_roll_count= micros();
  else  
    rc_roll= micros() - rc_roll_count;
}  

void handleInterrupt_Rtail()  
{  
  if(RTAIL_PIN_ON)  
    rc_rtail_count= micros();
  else  
    rc_rtail= micros() - rc_rtail_count;
} 

void handleInterrupt_Ltail()  
{  
  if(LTAIL_PIN_ON)
    rc_ltail_count= micros();
  else  
    rc_ltail= micros() - rc_ltail_count;
} 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Servo Setup Stuff
  pinMode(THROTTLE_PIN, INPUT);  
  pinMode(ROLL_PIN, INPUT);    
  pinMode(RTAIL_PIN, INPUT); 
  pinMode(LTAIL_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(THROTTLE_PIN),handleInterrupt_Throttle,CHANGE); // Catch up and down   
  attachInterrupt(digitalPinToInterrupt(ROLL_PIN),handleInterrupt_Roll,CHANGE); // Catch up and down   
  attachInterrupt(digitalPinToInterrupt(RTAIL_PIN),handleInterrupt_Rtail,CHANGE); // Catch up and down   
  attachInterrupt(digitalPinToInterrupt(LTAIL_PIN),handleInterrupt_Ltail,CHANGE); // Catch up and down

  current_time = micros();
}

void loop() {
  if((micros() - current_time) > 1000000){
    current_time = micros();
    Serial.println("Throttle");
    Serial.println(rc_throttle);
    Serial.println("Roll");
    Serial.println(rc_roll);
    Serial.println("Left Tail");
    Serial.println(rc_ltail);
    Serial.println("Right Tail");
    Serial.println(rc_rtail);    
  }
}
