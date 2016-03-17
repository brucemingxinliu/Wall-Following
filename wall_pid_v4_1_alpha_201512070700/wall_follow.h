/* Library for wall following robots.
	“If people aren't laughing at your dreams, your dreams aren't big enough.”
	“The fear of death follows from the fear of life. One who lives life fully is prepared to die at any time.” - Mark Twain
*/

#ifndef	WALLFOLLOW_H
#define WALLFOLLOW_H

class wallfollow : public Motor, public ultrasonic
{
// variables
public:
	float kp;		//
	float ki;		//	constants for pid
	float kd;		//
	float k_dist;	//
	float dist_to_wall;		//constant set point for wall follow
	float error;
	float weight;
	float angle_thresh;
	float correction;
	float total_error;
	char direction;
	float setPoint;
	float angle[2];
	float sensor_val[2];	
	int pwm;
	char ultra_side;
	float var_setPoint;

	int pwm1;
	int pwm2;
	int dir1;
	int dir2;
	int dir3;
	int dir4;
	int trigPinB;
	int echoPinB;
	int trigPinF;
	int echoPinF;
	int trigPinR;
	int echoPinR;

	Motor M1;	//motors
	Motor M2;

	ultrasonic U1;	//ultrsonic sensors
	ultrasonic U2;
	ultrasonic U3;

// functions
public:
	wallfollow();	//constructor.
	wallfollow(float dist_to_wall, float error, float weight, char ultra_side);	//parameterised constructor.
	void wallBegin();	//initialises the pinconfigs.
	void move(char dir);	//function for moving the bot in accordance to the distance from wall and PID values.
	int pid(float val1, float val2);	//calculation of control effort.
	int check_region(float val1, float val2);
	void setPoint_update();
	void read_sensor();	// reads sensors with wall following constraints.
	void follow();	//	go follow your heart( Oops! the wall I mean).
	void moveNinty();	//	turn ninty and stop.
};

#endif