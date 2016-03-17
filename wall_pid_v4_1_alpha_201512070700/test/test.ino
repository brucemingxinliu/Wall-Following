#include <Arduino.h>
#include "ultrasonic.h"
ultrasonic u;
void setup(){
	Serial.begin(9600);
	u.ultrasonicAttach(3, 4);
}

void loop(){
	float dist = u.getDistance();
	if(dist < 140 )
		Serial.println(dist);
	else
		Serial.println("4000");
}
