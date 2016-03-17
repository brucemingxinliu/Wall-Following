/* 	Library for obtaining distance with ultrasonic sensor HC-SR04 and Arduino
	Library tested 9th Dec,
*/


#include <Arduino.h>
#include <ultrasonic.h>

ultrasonic :: ultrasonic(){
	trigPin = -1;	//setting a default trigger pin
	echoPin = -1;	//setting up a default echo pin
}

void ultrasonic :: ultrasonicAttach(int trig, int echo){
	trigPin = trig;	//initializing trigger pin
	echoPin = echo;	//initializing echo pin.
}

float ultrasonic :: getDistance(){
	float ct, dist ;	//initialize local variables.
	digitalWrite(trigPin,HIGH);	//send a high pulse.
	delayMicroseconds(10);	//for 10 microseconds.
	digitalWrite(trigPin,LOW);	//bring trigger pin to LOW again.
	ct = pulseIn(echoPin,HIGH);	//read echo value.
	//ct = pulseIn(echoPin, HIGH, time_in_microseconds)	//change the time if need be of lower waiting times.
  	dist=ct/58.2;	//calculate distance based on time taken by sound to go and return from object.
  	return dist;
}