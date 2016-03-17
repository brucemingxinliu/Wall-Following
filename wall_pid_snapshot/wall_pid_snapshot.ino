//“If people aren't laughing at your dreams, your dreams aren't big enough.”

//“The fear of death follows from the fear of life. One who lives life fully is prepared to die at any time.” - Mark Twain

#define kp 13.5
#define ki 0.05
#define kd 0.00

#define dist_to_wall 18.0
#define error 2.5
#define pwm 65
#define weight 0.5

float inter_distance = dist_to_wall ; 

#define pwm1 5
#define dir1 9                         //for Right Motor
#define dir2 10
#define pwm2 6
#define dir3 8                        //for Left Motor
#define dir4 7                                      //for motor driver with 2 dir pins

#define trigPin1 3
#define echoPin1 2
#define trigPin2 13
#define echoPin2 12

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
  
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);

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
    //check_vals();
    chutiyaap_dobara();
    angle[0] = angle[1];
    angle[1] = sensor_val[1] - sensor_val[0];    
    
    if(sensor_val[1]!=4000 || sensor_val[0]!=4000){
      if(check_region(sensor_val[0], sensor_val[1]) == 0)
      {
        move('a');
      }
      if(check_region(sensor_val[0], sensor_val[1]) == 2)
      {
        if(angle[1] < -1.5)
          move('a');
        else move('c');
      }
        
      if(check_region(sensor_val[0], sensor_val[1]) == 1)
        if(angle[1] < -1.5)
          move('a');
        else move('f');
    
    Serial.print(angle[1]);  
    Serial.println();
    }
}

void move(char dir)
{
  if(dir == 'f')
  {
    direction='f';
    Serial.print("********move forward*********");
    Serial.print('\t');
    if(check_region(sensor_val[0],sensor_val[0]) == 0)
      move('a');
    else if((check_region(sensor_val[0],sensor_val[0]) == 1) && (check_region(sensor_val[1],sensor_val[1]) == 1 ))
    {
      analogWrite(pwm1,pwm);
      analogWrite(pwm2,pwm);                                             //for motor driver with 2 dir pins
    }
    else move('c');
  }
  if(dir == 'c'){
      direction='c';
      Serial.print("********move close*********");
      Serial.print('\t');
      analogWrite(pwm2, pwm  + pid(sensor_val[0],sensor_val[1]));
      analogWrite(pwm1, pwm);
    }
  if(dir == 'a'){
      direction='a';
      Serial.print("********move away*********");
      Serial.print('\t');
      analogWrite(pwm2, pwm);
      analogWrite(pwm1, pwm + pid(sensor_val[0],sensor_val[1]));
    }
}

int pid(float val1,float val2)
{
  angle[0] = angle[1];
  angle[1] = sensor_val[1] - sensor_val[0];
  total_error += (angle[1] - angle[0]);
  if(angle[1] == 0)
    return (30-pwm);
  else
         correction = kp * angle[1] + ki * total_error + kd * (angle[1] - angle[0]);         //Serial.print("correction: ")
        if(correction > 60 || correction < -60) return 60;
        else{
            if(correction<0) return ((int) (-1*correction));
        else return ((int) (correction));
        Serial.print(ki*total_error);
        Serial.print('\t');
  }
}

float read_sensor1(){
float ct, dist ;
digitalWrite(trigPin1,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1,LOW);
ct=pulseIn(echoPin1,HIGH);
  dist=ct/58.2;
  if (dist==0.0||dist>=300.0){
    dist=4000.0;
  }
  Serial.println(dist);
  return dist;
}


float read_sensor2(){
float ct, dist ;
digitalWrite(trigPin2,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2,LOW);
ct=pulseIn(echoPin2,HIGH);
  dist=ct/58.2;
  if (dist==0.0||dist>=300.0){
    dist=4000.0;
  }
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

/*void check_vals(){
  sensor_val[0] = read_sensor1();
  sensor_val[1] = read_sensor2();
  angle[0] = angle[1];
  angle[1] = sensor_val[1] - sensor_val[0];
  if(angle[1] - angle[0] > 10)
    check_vals();
  
  }*/
