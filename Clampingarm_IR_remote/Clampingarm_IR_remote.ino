//********************************************************************************
// * File Name          : Clampingarm_IR_remote.ino
// * Author             : RadioShack Corporation
// * Version            : V1.0
// * Date               : 2014/01/23
// * Description        : Use the RadioShack Make: it Robotics Remote Control to control the clamping arm robot.
// ********************************************************************************
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, see http://www.gnu.org/licenses/
//
/*****************************************************************************
  Press the following buttons (SW)or combinations of buttons (SW + SW) to control the robot.
  SW1 forward
  SW3 backward
  SW4 turn right
  SW2 turn left
  SW5 + SW1 move arm up
  SW5 + SW3 move arm down
  SW6 + SW1 close clamp
  SW6 + SW3 open clamp
  SW7 clamp hold
  SW8 release clamp
******************************************************************************/
#include <MakeItRobotics.h> //include library
#include <IRremote.h>
#include <Streaming.h>
const int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
static boolean hold=false;  //flag to record the clamp status 

MakeItRobotics clampingarm; //declare object
// ***********************************************************************************************************
// *                            Power Up Init.
// ***********************************************************************************************************
void setup() 
{
    Serial.begin(10420);         //tell the Arduino to communicate with Make: it PCB
    //Serial.begin(9600); 
    clampingarm.remote_setup();  //remote control setup        
    delay(500);                  //delay500ms
    clampingarm.all_stop();      //stop all motors
    irrecv.enableIRIn();

}
// ***********************************************************************************************************
// *                            Main Loop 
// ***********************************************************************************************************
void loop() 
{ 
  
  if (irrecv.decode(&results)) {
       Serial.println(results.value, HEX);

       if (results.value == 0XFFFFFFFF)
          results.value = key_value;
       
      if(results.value==CW1)                       //Forward action
      {
        clampingarm.clampingarm_up(0);      //stop arm
        if(hold==false)                     //not hold status
          clampingarm.clampingarm_clamp(0); //release the clamp     
        clampingarm.go_forward(255);        //go forward
      }  
      else if(results.value==CW2)                  //Backward action
      {
        clampingarm.clampingarm_up(0);      //stop arm
        if(hold==false)                     //not hold status
          clampingarm.clampingarm_clamp(0); //release the clamp     
        clampingarm.go_backward(255);       //go backward
      }  
      else if(results.value==CW4)                  //Turn Left action
      {
        clampingarm.clampingarm_up(0);      //stop arm
        if(hold==false)                     //not hold status
          clampingarm.clampingarm_clamp(0); //release the clamping    
        clampingarm.turn_left(255);         //turn left   
      }  
      else if(results.value==CW3)                  //Turn Right action
      {
        clampingarm.clampingarm_up(0);      //stop arm
        if(hold==false)                     //not hold status
          clampingarm.clampingarm_clamp(0); //release the clamp     
        clampingarm.turn_right(255);        //turn right    
      }  
      else if(results.value==CW5)                 //Move Arm Up action
      {
        clampingarm.move_stop();            //stop wheels
        clampingarm.clampingarm_up(120);     //move arm up
        if(hold==false)                     //not hold status
          clampingarm.clampingarm_clamp(0); //release the clamp
      }  
      else if(results.value==CW6)                 //Move Arm Down action
      {
        clampingarm.move_stop();            //stop wheels
        clampingarm.clampingarm_down(120);   //move arm down
        if(hold==false)                     //not hold status
          clampingarm.clampingarm_clamp(0); //release the clamp
      }      

      /* THE MOTOR FOR CW7 & CW8 rotates slow */
      else if(results.value==CW7)                 //Close Clamp action
      {
        clampingarm.move_stop();            //stop wheels
        clampingarm.clampingarm_up(0);      //stop arm
        clampingarm.clampingarm_clamp(160);  //close clamp
        hold=false;                         //clear hold status
      }  
      else if(results.value==CW8)                 //Open Clamp action
      {
        clampingarm.move_stop();            //stop wheels
        //clampingarm.clampingarm_up(0);      //stop arm
        clampingarm.clampingarm_release(160);//release clamp 
        hold=false;                         //clear hold status
      }
      else if(results.value==CW9)                  //Clamp Hold action
      {
        hold=true;                          //enable hold status
        clampingarm.clampingarm_clamp(80);  //hold clamp    
      }  
      else if(results.value==CW10)                  //Release Clamp action
      {
        hold=false;                         //clear hold status
        clampingarm.clampingarm_clamp(0);   //release clamp      
      }      
      else                                  //if no buttons are pushed
      {
        clampingarm.move_stop();            //stop all motors
        //clampingarm.clampingarm_up(0);      //stop arm up down
        if(hold==false)                     //not hold status
          clampingarm.clampingarm_release(0);  //release clamp             
      }  
      key_value = results.value;
      irrecv.resume(); // Receive the next value
  }  
}
/***********************************************************************
*    Remote Scan
************************************************************************/
ISR(PCINT0_vect)  //interrupt code
{
  clampingarm.remote_scan();  //analyze signal from RadioShack Make: it Robotics Remote Control
}
