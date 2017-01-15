/*
 * Copyright (C) 2012-2016 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>
#include <google/protobuf/message.h>
#include <iostream>
#include <math.h>
#include "ArduinoComms.hh"


#define RAD2DEG (180/M_PI);

using namespace std;
volatile float yaw,roll,pitch,rollRate,pitchRate,yawRate;

/////////////////////////////////////////////////
// Function is called everytime a message is received.
void cb(ConstIMUPtr &_msg) //CessnaPtr
{
    // Dump the message contents to stdout
    //std::cout << _msg->DebugString();

    //ORIENTATION - Quaternion Math
    double q0 = (double)(_msg->orientation().x());
    double q1 = (double)(_msg->orientation().y());
    double q2 = (double)(_msg->orientation().z());
    double q3 = (double)(_msg->orientation().w());
   
    yaw = (float)atan(2*(q0*q1 +
    q3*q2)/(pow(q3,2)-pow(q2,2)-pow(q1,2)+pow(q0,2))) * RAD2DEG;
    roll = (float)asin(-2*(q0*q2-q1*q3)) * RAD2DEG;
    pitch = (float)
    atan(2*(q1*q2+q0*q3)/(pow(q3,2)+pow(q2,2)-pow(q1,2)-pow(q0,2))) * RAD2DEG;
    /* DEBUG data
    cout << "Q0, Q1, Q2, Q3: " << q0 << ", " << q1 << ", " << q2 << ", " << q3 << endl;
    cout << "ROLL: " << roll << endl;
    cout << "PITCH: " << pitch << endl;
    cout << "YAW: " << yaw << endl;
    */

    //ANGULAR VELOCITY
    rollRate = (float)(_msg->angular_velocity().x());
    pitchRate = (float)(_msg->angular_velocity().y());
    yawRate = (float)(_msg->angular_velocity().z());
    /* DEBUG data
    cout << "RollRate: " << rollRate << endl;
    cout << "PitchRate: " << pitchRate << endl;
    cout << "YawRate: " << yawRate << endl;
    */

    

}

/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
    // Load gazebo
    gazebo::client::setup(_argc, _argv);

    //Initialize Arduino
    Arduino a("/dev/ttyACM0");

    // Create our node for communication
    gazebo::transport::NodePtr node(new gazebo::transport::Node());
    node->Init();

    // Listen to Gazebo Cessna State topic
    gazebo::transport::SubscriberPtr sub =
    node->Subscribe("~/cessna_c172/cessna_c172/body/Vilma_IMU/imu", cb); 

//Topics of interest: ~/cessna_c172/state, ~/cessna_c172/control,
//~cessna_c172/cessna_172/body/Vilma_IMU/imu
   
    // Busy wait loop...replace with your own code as needed.
    while (true){
        ArduinoPacket* ap = a.loadPacket(rollRate, pitchRate, yawRate, yaw, pitch, roll, 0, 0,0);
        cout << ap->data.pitch << endl;
        a.sendData(ap);
        gazebo::common::Time::MSleep(10);
    }

    // Make sure to shut everything down.
    gazebo::client::shutdown();
}
