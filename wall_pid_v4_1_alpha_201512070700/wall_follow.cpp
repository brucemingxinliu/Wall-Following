#include <Arduino.h>
#include "wall_follow.h"
#include "ultrasonic.h"
#include "motor.h"
#include "newmotor.h"

wallfollow :: wallfollow(){
	kp = 3.0;
	ki = 0.005;
	kd = 0.00;
	k_dist = 0.00;
	dist_to_wall = 30.00;
	error = 2.50;
	weight = 0.00;
	angle_thresh = -2.25;
	correction = 0.00;
	total_error = 0.00;
	direction = 'f';
	setPoint = 0.00;
	angle[2] = {0.00, 0.00};
	sensor_val[2] = {24.0, 24.0};
	pwm = 100;
	ultra_side = 'r';
	var_setPoint = 30.00;
	M1.attachMotor(-1, -1, -1);
	U1.ultrasonicAttach(-1, -1);
	U2.ultrasonicAttach(-1, -1);

	int pwm1 = -1;
	int pwm2 = -1;
	int dir1 = -1;
	int dir2 = -1;
	int dir3 = -1;
	int dir4 = -1;
	int trigPinM = -1;
	int echoPinM = -1;
	int trigPinF = -1;
	int echoPinF = -1;
	int trigPinR = -1;
	int echoPinR = -1;

}

wallfollow :: wallfollow(float dist_to_wall, float error, float weight, char ultra_side){
	kp = 3.0;
	ki = 0.005;
	kd = 0.00;
	k_dist = 0.00;
	dist_to_wall = 30.00;
	error = 2.50;
	weight = 0.00;
	angle_thresh = -2.25;
	correction = 0.00;
	total_error = 0.00;
	direction = 'f';
	setPoint = 0.00;
	angle[2] = {0.00, 0.00};
	sensor_val[2] = {24.0, 24.0};
	pwm = 100;
	ultra_side = 'r';
	var_setPoint = 30.00;
	M1.attachMotor(-1, -1, -1);
	M2.attachMotor(-1, -1, -1);
	U1.ultrasonicAttach(-1, -1);
	U2.ultrasonicAttach(-1, -1);
	pwm1 = -1;
	pwm2 = -1;
	dir1 = -1;
	dir2 = -1;
	dir3 = -1;
	dir4 = -1;
	trigPinM = -1;
	echoPinM = -1;
	trigPinF = -1;
	echoPinF = -1;
	trigPinR = -1;
	echoPinR = -1;
}

void wallfollow :: wallBegin(dir1, dir2, pwm1, dir3, dir4, pwm2, trigPinF, echoPinF, trigPinM, echoPinM, trigPinR, echoPinR){


	this->trigPinR = trigPinR;
	this->echoPinR = echoPinR;
	this->trigPinF = trigPinF;
	this->echoPinF = echoPinF;
	this->trigPinM = trigPinM;
	this->echoPinM = echoPinM;
	this->dir1 = dir1;
	this->dir2 = dir2;
	this->dir3 = dir3;
	this->dir4 = dir4;


	M1.attachMotor(dir1, dir2, pwm1);
	M2.attachMotor(dir3, dir4, pwm2);
	U1.ultrasonicAttach(trigPinM, echoPinM);
	U2.ultrasonicAttach(trigPinF, echoPinF);
	Serial.print("Sensor1");
    Serial.print('\t');
    Serial.print("Sensor2");
    Serial.print('\t');
    Serial.print("I_D");
    Serial.print('\t');
    Serial.print("Angle");
    Serial.print('\t');
    Serial.print("*********Direction*********");
    Serial.print('\t');
    Serial.print("correction");
    Serial.println();
    Serial.println();
}

void wallfollow :: move(char dir){
  
  if(dir == 'f'){
    	direction='f';
    	Serial.print("********move forward*********");
    	Serial.print('\t');
    	pid(sensor_val[0], sensor_val[1]);
    	M1.moveMotor(pwm);
    	M2.moveMotor(pwm);
	}
  if(dir == 'c'){
      direction='c';
      Serial.print("********move close*********");
      Serial.print('\t');
      M1.moveMotor(pwm + pid(sensor_val[0], sensor_val[1]));
      M2.moveMotor(2*pwm/3);
  	}

  if(dir == 'a'){
      direction='a';
      Serial.print("********move away*********");
      Serial.print('\t');
      M1.moveMotor(2*pwm/3);
      M2.moveMotor(9*pwm/8 + pid(sensor_val[0], sensor_val[1]));
    }
    if(dir == 'n'){
      direction='n';
      Serial.print("********turn 90*********");
      Serial.print('\t');
      M1.moveMotor(pwm/2 + 8);
      M2.moveMotor(0);
    }
}

int wallfollow :: pid(float val1, float val2){
	angle[0] = angle[1];
  	angle[1] = sensor_val[1] - sensor_val[0];
  	total_error += (angle[1] - angle[0]);
  	if(angle[1] == 0){
    	Serial.print(20-pwm);
    	return (20-pwm);
  	}
  
  	else
    correction = kp * angle[1] + ki * total_error + kd * (angle[1] - angle[0]) + abs(k_dist * (var_setPoint - (sensor_val[0] + sensor_val[1]))/2);        
    Serial.print(correction);
    if(correction > 9 || correction < -9)
    	return 9;
    else
        if(correction<0)
        	return ((int) (-1*correction));
        else
        	return ((int) (correction));
}

int wallfollow :: check_region(float val1, float val2){
	int dist = (val1 + val2)/2;
  	if(dist < var_setPoint - error)
    	return 0;
  	if(dist > var_setPoint + error)
    	return 2;
  	else return 1;
}

void wallfollow :: setPoint_update(void ){
	var_setPoint = weight * (sensor_val[0] + sensor_val[1])/2 + (1-weight) * dist_to_wall; 
    Serial.print(var_setPoint);
    Serial.print('\t');
}

void wallfollow :: read_sensor(){
	sensor_val[0] = U1.getDistance()
  	if (sensor_val[0] == 0.0|| sensor_val[0] >= 100.0){
    	sensor_val[0] = 4000.0;
  	}
  	Serial.print(sensor_val[0]);
  	Serial.print('\t');

  	sensor_val[1] = U2.getDistance()
  	if (sensor_val[1] == 0.0|| sensor_val[1] >= 100.0){
    	sensor_val[1] = 4000.0;
  	}
  	Serial.print(sensor_val[1]);
  	Serial.print('\t');
}

void wallfollow :: follow(){

	if( Serial.available() ){
		
      	inst = Serial.read();
      	M1.attachMotor(dir2, dir1, pwm1);
		M2.attachMotor(dir4, dir3, pwm2);

      	trigPinF = trigPinR;
      	echoPinF = echoPinR;
      	dist_to_wall += 10.0;
      	Serial.println("moving in reverse");
    }

    read_sensor();
    setPoint_update();
    angle[0] = angle[1];
    angle[1] = sensor_val[1] - sensor_val[0];
    Serial.print(angle[1]);
    Serial.print('\t');
    
    if(sensor_val[1] == 4000){
    	moveNinty();
    }
    
    else{
	    if(check_region(sensor_val[0], sensor_val[1]) == 0){
          	if(angle[1] > -angle_thresh)
            	move('c');
          	else move('a');
        }
        if(check_region(sensor_val[0], sensor_val[1]) == 2){

         	if(angle[1] < angle_thresh)
           		move('a');
          	else move('c');
        }
        
        if(check_region(sensor_val[0], sensor_val[1]) == 1)
        	if(angle[1] < angle_thresh)
            	move('a');
          	else if(angle[1] > -angle_thresh)
            	move('c');
          	else move('f');
      	}
      	Serial.println();
}




void wallfollow :: moveNinty(){
	read_sensor();
    setPoint_update();
    angle[0] = angle[1];
    angle[1] = sensor_val[1] - sensor_val[0];
    Serial.print(angle[1]);
    Serial.print('\t');
    
    if(sensor_val[1] == 4000){
    	move('n');
      	while(abs(angle[1]) > 1.0){
        
    		move('n');
        	read_sensor();
        	angle[1] = sensor_val[1] - sensor_val[0];
        	while(sensor_val[0] == 4000){
        		read_sensor();
          		move('c');
          		Serial.println();
        	}
    	}
    	flag = 1;
    }
    if(flag == 1 && abs(angle[1]) <= 2){
    	Serial.println("**********STOPPED***********");
    	M1.moveMotor(HIGH, HIGH);
    	M2.moveMotor(HIGH, HIGH);
    }
}	