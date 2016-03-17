#include <Arduino.h>
//#include <ultrasonic.h>
#include "ultrasonic.h"
ultrasonic :: ultrasonic(){
	//trigPin = 3;
	//echoPin = 4;
        ultrasonicAttach(3, 4);
}

void ultrasonic :: ultrasonicAttach(int trig, int echo){
	trigPin = trig;												//initializing trigger pin
	echoPin = echo;												//initializing echo pin	
}

float ultrasonic :: getDistance(){
	float ct, dist ;											//initialize local variables
	digitalWrite(trigPin,HIGH);									//send a high pulse
	delayMicroseconds(10);										//for 10 microseconds
	digitalWrite(trigPin,LOW);									//bring trigger pin to LOW again
	ct=pulseIn(echoPin,HIGH);									//read echo value
  	dist=ct/58.2;												//calculate distance
  	return dist;
}
