/*library tested 30 NOVEMBER
  Made by Deep Goel and Kartik Gupta
*/

#include <arduino.h>
#include "motor.h"
#include "botconfig.h"
void setup()
{
  Serial.begin(9600);
  Motor testmotor;
  
      						//constructor
//Motor(int Dir1,int Dir2,int Pwm)	//constructor with attachments of pins
  testmotor.attachMotor(DIR1_PIN,DIR2_PIN ,PWM_PIN); //attachments of pins
//testmotor.moveMotor(200);			//+ve for CW and -ve for CCW.
//testmotor.moveMotor(0,1,100);                  // dir1 and dir2 can be 1 or 0,pwm can only be +ve for CWstopMotor();				
//delay(2000);
//testmotor.stopMotor();                        //By default stop motor will lock motor
//testmotor.lockMotor();				//To lock the motor
//testmotor.freeMotor();				//Free the motor
//testmotor.setMeanSpeed(255);	//Sets the meanspeed with which motor moves when speed=100%	
//testmotor.setMotorSpeed(-50);       //+ve for CW and -ve for CCW. Speed in percentage of meanspeed
//testmotor.setMotorSpeed(0,1,50); // dir1 and dir2 can be 1 or 0,speed is only positive
//testmotor.changePWM(200);		//Just to change the PWM in whatever direction the motor was moving
//testmotor.changeSpeed(100);	//Just to change the speed (In percentage) not the direction
//Serial.println(testmotor.getDirection());             //+1 for cw and -1 for ccw and 0 for free or locked
//Serial.println(testmotor.isFree());                  //+1 for free and 0 for not free
//Serial.println(testmotor.isLocked());//+1 for locked and 0 for not locked
//Serial.println(testmotor.getSpeed());                // returns speed in % of mean speed
//Serial.print( testmotor.getPWM());                  // returns +ve for CW and -ve for CCW.
  testmotor.startSmoothly(200);   //+ve for CW and -ve for CCW.
  delay(5000);
  Serial.println( testmotor.getPWM());
  testmotor.stopSmoothly();
 
}
void loop(){
}
