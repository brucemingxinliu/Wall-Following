/* Library for obtaining distance with ultrasonic sensor
HC-SR04 and Arduino
*/

#ifndef	ULTRASONIC_H
#define ULTRASONIC_H

class ultrasonic
{
// variables
public:
	int trigPin;
	int echoPin;
// functions
public:
	ultrasonic();
	void ultrasonicAttach(int trig, int echo);	// attach pins to sensors
	float getDistance();		// reads distance from ultarsonic sensors 
};

#endif