
//PWM Setup for RC Controller
volatile long rc_throttle; // pwm value for the throttle input
volatile long rc_throttle_count; // temporary variable used for counting the length of the pwm signal

volatile long rc_roll; // servo value  
volatile long count1; // temporary variable for servo1

volatile long servo2; // servo value  
volatile long count2; // temporary variable for servo2  

volatile long servo3; // servo value  
volatile long count3; // temporary variable for servo3

void handleInterrupt_P2()  
{  
  if(int0)  
    count0=micros(); // we got a positive edge  
  else  
    servo0=micros()-count0; // Negative edge: get pulsewidth  
}

void handleInterrupt_P3()  
{  
  if(int1)  
    count1=micros(); // we got a positive edge  
  else  
    servo1=micros()-count1; // Negative edge: get pulsewidth  
}  

void handleInterrupt_P18()  
{  
  if(int2)  
    count2=micros(); // we got a positive edge  
  else  
    servo2=micros()-count2; // Negative edge: get pulsewidth  
} 

  void handleInterrupt_P19()  
{  
  if(int3)  
    count3=micros(); // we got a positive edge  
  else  
    servo3=micros()-count3; // Negative edge: get pulsewidth  
} 

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
