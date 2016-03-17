//DONOT TOUCH THIS CODE.

//“If people aren't laughing at your dreams, your dreams aren't big enough.”

//“The fear of death follows from the fear of life. One who lives life fully is prepared to die at any time.” - Mark Twain

#define kp 3.00                          
#define ki 0.00
#define kd 0.00
#define k_dist 0.0

#define dist_to_wall 30.0
#define error 2.5
#define pwm 11
#define weight 0.0
#define angle_thresh -2.25

float var_setPoint = dist_to_wall; 

#define pwm1 5            // Left Motor
#define dir1 9
#define dir2 10

#define pwm2 6            // Right Motor
#define dir3 8
#define dir4 7

#define trigPinB 3
#define echoPinB 2
#define trigPinF 13
#define echoPinF 12
#define trigPinR 11
#define echoPinR 4

volatile float sensor_val[2];
float correction = 0;
volatile float angle[2] = {0,0};
float total_error = 0;
char direction;
// float setPoint;

void setup(){

  Serial.begin(9600);

    pinMode(pwm1,OUTPUT);
    pinMode(dir1,OUTPUT);
    pinMode(dir2,OUTPUT);
    pinMode(pwm2,OUTPUT);
    pinMode(dir3,OUTPUT);
    pinMode(dir4,OUTPUT);                         //for motor driver with 2 dir pins
  
    pinMode(trigPinB,OUTPUT);
    pinMode(echoPinB,INPUT);
    pinMode(trigPinF,OUTPUT);
    pinMode(echoPinF,INPUT);
    
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,LOW);

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

void loop()
{
    sensor_val[0] = read_sensor(trigPinB, echoPinB);
    sensor_val[1] = read_sensor(trigPinF, echoPinF);
    setPoint_update();
    angle[0] = angle[1];
    angle[1] = sensor_val[1] - sensor_val[0];
    Serial.print(angle[1]);
    Serial.print('\t');
    
    if(sensor_val[1] == 4000){
      move('n');
      while(abs(angle[1]) > 2){
        Serial.print("Code part 2 running");
        move('n');
        sensor_val[0] = read_sensor(trigPinB, echoPinB);
        sensor_val[1] = read_sensor(trigPinF, echoPinF);
        angle[1] = sensor_val[1] - sensor_val[0];
        while(read_sensor(trigPinB, echoPinB) == 4000){
          move('c');
          Serial.println();
        }
      }
    }
    
    else
    {
      if(check_region(sensor_val[0], sensor_val[1]) == 0)
      {
        if(angle[1] > -angle_thresh)
          move('c');
        else move('a');
      }
      if(check_region(sensor_val[0], sensor_val[1]) == 2)
      {
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

void move(char dir)
{
  if(dir == 'f')
  {
    direction='f';
    Serial.print("********move forward*********");
    Serial.print('\t');
    pid(sensor_val[0],sensor_val[1]);
    analogWrite(pwm1,pwm);
    analogWrite(pwm2,pwm);                                             //for motor driver with 2 dir pins
  }
  if(dir == 'c'){
      direction='c';
      Serial.print("********move close*********");
      Serial.print('\t');
      analogWrite(pwm2, 2*pwm/3);
      analogWrite(pwm1,  pwm + pid(sensor_val[0],sensor_val[1]));
      //analogWrite(pwm1, 0);

    }
  if(dir == 'a'){
      direction='a';
      Serial.print("********move away*********");
      Serial.print('\t');
      analogWrite(pwm2, 9*pwm/8 + pid(sensor_val[0],sensor_val[1]));
      //analogWrite(pwm2, 0);
      analogWrite(pwm1, 2*pwm/3);
    }
    if(dir == 'n'){
      direction='n';
      Serial.print("********turn 90*********");
      Serial.print('\t');
      analogWrite(pwm2,0);
      analogWrite(pwm1,  pwm/2 + 8);
    }
}

int pid(float val1,float val2)
{
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
          if(correction > 9 || correction < -9) return 9;
          else
              if(correction<0) return ((int) (-1*correction));
              else return ((int) (correction));
  }


float read_sensor(int trigPin, int echoPin ){
float ct, dist ;
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);
ct=pulseIn(echoPin,HIGH);
  dist=ct/58.2;
  if (dist==0.0||dist>=100.0){
    dist=4000.0;
  }
  Serial.print(dist);
  Serial.print('\t');
  return dist;
}

int check_region(float val1, float val2){
  int dist = (val1 + val2)/2;
  if(dist < var_setPoint - error)
    return 0;
  if(dist > var_setPoint + error)
    return 2;
  else return 1;
}

void setPoint_update(){
    var_setPoint = weight * (sensor_val[0] + sensor_val[1])/2 + (1-weight) * dist_to_wall; 
    Serial.print(var_setPoint);
    Serial.print('\t');
}
