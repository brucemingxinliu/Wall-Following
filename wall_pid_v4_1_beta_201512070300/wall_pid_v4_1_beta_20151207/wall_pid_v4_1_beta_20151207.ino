//snapshot

//“If people aren't laughing at your dreams, your dreams aren't big enough.”

//“The fear of death follows from the fear of life. One who lives life fully is prepared to die at any time.” - Mark Twain

#define kp 5.00                                            
#define ki 0.00
#define kd 0.00
#define k_dist 0.5

#define dist_to_wall 40.0
#define error 2.5
#define pwm 20
#define weight 0.5
#define angle_thresh -1.5

float inter_distance = dist_to_wall ; 

#define pwm1 5
#define dir1 10                         //for Right Motor
#define dir2 9
#define pwm2 6
#define dir3 8                        //for Left Motor
#define dir4 7                                     //for motor driver with 2 dir pins

#define trigPinB 3
#define echoPinB 2
#define trigPinF 13
#define echoPinF 12

volatile float sensor_val[2];
float correction = 0;
volatile float angle[2] = {0,0};
float total_error = 0;
char direction;
float setPoint;

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

  Serial.print("Sensor1");
  Serial.print('\t');
  Serial.print("Sensor2");
  Serial.print('\t');
  Serial.print("*********Direction*********");
  Serial.print('\t');
  Serial.print("correction");
  Serial.print('\t');
  Serial.print("correction");
  Serial.print('\t');
  Serial.println();

  digitalWrite(dir1,HIGH);
  digitalWrite(dir2,LOW);
  digitalWrite(dir3,HIGH);
  digitalWrite(dir4,LOW);
}

void loop()
{
    Serial.print("Sensor1");
    Serial.print('\t');
    Serial.print("Sensor2");
    Serial.print('\t');
    Serial.print("Intermediate Dist");
    Serial.print('\t');
    Serial.print("*********Direction*********");
    Serial.print('\t');
    Serial.print("correction");
    Serial.print('\t');
    Serial.print("correction");
    Serial.print('\t');
    Serial.println();
    
    //wall_shift();
    chutiyaap_dobara();
    angle[0] = angle[1];
    angle[1] = sensor_val[1] - sensor_val[0];    
    
    if(sensor_val[1]!=4000 || sensor_val[0]!=4000){
      if(check_region(sensor_val[0], sensor_val[1]) == 0)
      {
        if(angle[1] > -angle_thresh)
          move('c');
        else move('a');
      }
      if(check_region(sensor_val[0], sensor_val[1]) == 2)
      {
//        if(angle[1] < angle_thresh)
//          move('a');
         move('c');
      }
        
      if(check_region(sensor_val[0], sensor_val[1]) == 1)
        if(angle[1] < angle_thresh)
          move('a');
        else if(angle[1] > -angle_thresh)
          move('c');
        else move('f');
    }else move('f');
    Serial.println();
}

void move(char dir)
{
  if(dir == 'f')
  {
    direction='f';
    Serial.print("********move forward*********");
    Serial.print('\t');
    analogWrite(pwm1,pwm);
    analogWrite(pwm2,pwm);                                             //for motor driver with 2 dir pins
  }
  if(dir == 'c'){
      direction='c';
      Serial.print("********move close*********");
      Serial.print('\t');
      analogWrite(pwm2, pwm + pid(sensor_val[0],sensor_val[1]) + pid_dist());
      analogWrite(pwm1, pwm);
    }
  if(dir == 'a'){
      direction='a';
      Serial.print("********move away*********");
      Serial.print('\t');
      analogWrite(pwm2, pwm);
      analogWrite(pwm1, pwm + pid(sensor_val[0],sensor_val[1]) + pid_dist());
    }
}

int pid(float val1,float val2)
{
  angle[0] = angle[1];
  angle[1] = sensor_val[1] - sensor_val[0];
  total_error += (angle[1] - angle[0]);
  if(angle[1] == 0)
    return (10-pwm);
  else
         correction = kp * angle[1] + ki * total_error + kd * (angle[1] - angle[0]);        
         Serial.print("correction: ");
         Serial.println(correction);
        if(correction > 15 || correction < -15) return 15;
        else
            if(correction<0) return ((int) (-1*correction));
            else return ((int) (correction));
        // ````    Serial.print(ki*total_error);
  }


float pid_dist(){
  float correct = k_dist * (inter_distance - chutiyaap(sensor_val[0] , sensor_val[1]));
  if(correct < 0) return (-1 * correct);
  return correct;
}

float read_sensor1(){
float ct, dist ;
digitalWrite(trigPinB,HIGH);
delayMicroseconds(10);
digitalWrite(trigPinB,LOW);
ct=pulseIn(echoPinB,HIGH);
  dist=ct/58.2;
  if (dist==0.0||dist>=140.0){
    dist=4000.0;
  }
  Serial.print("sensor1 = ");
  Serial.print(dist);
  Serial.print("\t");
  return dist;
}


float read_sensor2(){
float ct, dist ;
digitalWrite(trigPinF,HIGH);
delayMicroseconds(10);
digitalWrite(trigPinF,LOW);
ct=pulseIn(echoPinF,HIGH);
  dist=ct/58.2;
  if (dist==0.0||dist>=140.0){
    dist=4000.0;
  }
  Serial.print("sensor2 = ");
  Serial.println(dist);
  return dist ;
}

int check_region(float val1, float val2){
  int dist = chutiyaap(val1 , val2);
  if(dist < inter_distance - error)
    return 0;
  if(dist > inter_distance + error)
    return 2;
  else return 1;
}

float chutiyaap(float val1, float val2){
    if(min(val1, val2) < max(val1 , val2) * 0.5 )
      return min(val1 , val2);
    return (val1 + val2)/2;
  }

void chutiyaap_dobara(){
    sensor_val[0] = read_sensor1();
    sensor_val[1] = read_sensor2();
    inter_distance = weight * chutiyaap(sensor_val[0],sensor_val[1]) + (1-weight) * dist_to_wall; 
    Serial.print("inter_distance :");
    Serial.println(inter_distance);
}
