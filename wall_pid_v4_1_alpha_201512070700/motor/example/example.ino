#include <arduino.h>
#include "motor.h"
#include "botconfig.h"
void setup()
{
  Serial.begin(9600);
  Motor testmotor;
  
      						//constructor
//    Motor(int Dir1,int Dir2,int Pwm)	//constructor with attachments of pins
      testmotor.attachMotor(1,2,3); //attachments of pins
//    moveMotor(int Pwm);			//+ve for CW and -ve for CCW.
//    moveMotor(int Dir1,int Dir2,int Pwm); // dir1 and dir2 can be 1 or 0,pwm can only be +ve for CWstopMotor();				//By default stop motor will lock motor
//    lockMotor();				//To lock the motor
//    freeMotor();				//Free the motor
//    setMeanSpeed(int Speed);	//Sets the meanspeed with which motor moves when speed=100%	
//    setMotorSpeed(int Speed);		//+ve for CW and -ve for CCW. Speed in percentage of meanspeed
//    setMotorSpeed(int Dir1,int Dir2,int Speed); // dir1 and dir2 can be 1 or 0
//    changePWM(int Pwm);		//Just to change the PWM in whatever direction the motor was moving
//    changeSpeed(int Speed);	//Just to change the speed (In percentage) not the direction
//    getDirection();             //+1 for cw and -1 for ccw and 0 for free or locked
//    isFree();                  //+1 for free and 0 for not free
//    isLocked();                //+1 for locked and 0 for not locked
//    getSpeed();                // returns speed in % of mean speed
//    getPWM();                  // returns +ve for CW and -ve for CCW.
//    startSmoothly(int Pwm);   //+ve for CW and -ve for CCW.
//    stopSmoothly();

}
void loop(){
}
