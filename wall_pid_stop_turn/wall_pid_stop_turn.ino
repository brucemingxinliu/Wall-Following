//“If people aren't laughing at your dreams, your dreams aren't big enough.”

//3 sensors with pid
//Date-26/11/2015

//“The fear of death follows from the fear of life. One who lives life fully is prepared to die at any time.” - Mark Twain

#define kp 1.0
#define ki 0.8
#define kd 0.00

#define dist_to_wall 25.0
#define error 5.00 
#define pwm 100


#define pwm1 5
#define dir1 9
#define dir2 10
#define pwm2 6
#define dir3 8
#define dir4 7                                      //for motor driver with 2 dir pins

#define trigPin1 3
#define echoPin1 2
#define trigPin2 13
#define echoPin2 12

volatile float sensor_val[2];
float correction = 0;
volatile float deviation[2] = {0,0};
volatile float angle[2] = {0,0};
float total_error = 0;
char direction;
float setPoint;

void setup(){
  Serial.begin(9600);

  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);

  pinMode(pwm1,OUTPUT);
  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(dir3,OUTPUT);
  pinMode(dir4,OUTPUT);                         //for motor driver with 2 dir pins
  
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

  
    sensor_val[0]= read_sensor1();
    sensor_val[1]=read_sensor2(); 
    angle[0] = angle[1];
    angle[1] = sensor_val[1] - sensor_val[0];
    Serial.print("angle :");
    Serial.println(angle[1]);
}

void loop()
{
    sensor_val[0] = read_sensor1();
    sensor_val[1] = read_sensor2();
     angle[0] = angle[1];
    angle[1] = sensor_val[1] - sensor_val[0];
    Serial.print("angle :");
    Serial.println(angle[1]); 
    if(check_region(sensor_val[0], sensor_val[1]) == 0){
       while(angle[1] < 2.0)
        {
          setPoint = 2.0;
          move('a');
        }
        move('f');
    }

    else if(check_region(sensor_val[0], sensor_val[1]) == 1){
       while(angle[1] > 1.2 )
        {
          setPoint = 1.2;
          move('c');
        }
        while(angle[1] < -1.2 )
        {
          setPoint = -1.2;
          move('a');
        }
        move('f');
        
    }
    else if(check_region(sensor_val[0], sensor_val[1]) == 2){

       while( angle[1] > -2.0){
        setPoint = -2.0;
        move('c');
      }
     move('f');
    }
    delay(10);
}


void move(char dir)
{
  if(dir == 'f'){
    direction='f';
    Serial.print("********move forward*********");
    Serial.println();
      digitalWrite(dir1,HIGH);
      digitalWrite(dir2,LOW);
      analogWrite(pwm1,pwm);
      digitalWrite(dir3,HIGH);
      digitalWrite(dir4,LOW);
      analogWrite(pwm2,pwm); 
     sensor_val[0] = read_sensor1();
     sensor_val[1] = read_sensor2();                        //for motor driver with 2 dir pins
     angle[0] = angle[1];
     angle[1] = sensor_val[1] - sensor_val[0];
     Serial.print("angle :");
     Serial.println(angle[1]);
      
                                                                         //move both motors forward
    
  }
   else if(dir == 'c'){
      direction='c';
      Serial.print("********move close*********");
      Serial.println();
      
      digitalWrite(dir1,HIGH);
      digitalWrite(dir2,LOW);
      analogWrite(pwm1,75 - pid(sensor_val[0],sensor_val[1]));
      digitalWrite(dir3,LOW);
      digitalWrite(dir4,HIGH);
      analogWrite(pwm2,75 - pid(sensor_val[0],sensor_val[1]));  //for motor driver with 2 dir pins

                                                                          //different PWMs
  }
  else if(dir == 'a'){
      direction='a';
      Serial.print("********move away*********");
      Serial.println();
      
      digitalWrite(dir1,LOW);
      digitalWrite(dir2,HIGH);
      analogWrite(pwm1,60 + pid(sensor_val[0],sensor_val[1]));
      digitalWrite(dir3,HIGH);
      digitalWrite(dir4,LOW);
      analogWrite(pwm2,60 + pid(sensor_val[0],sensor_val[1]));  //for motor driver with 2 dir pins

                                                                          //different PWMs
  }
}

int pid(float val1 , float val2)
{ 
   sensor_val[0]= read_sensor1();
    sensor_val[1]=read_sensor2(); 
     angle[0] = angle[1];
     angle[1] = sensor_val[1] - sensor_val[0];
     Serial.print("angle :");
     Serial.println(angle[1]);
  deviation[0] = deviation[1];
  deviation[1] = setPoint - angle[1];
  Serial.print("deviation :");
  Serial.println(deviation[1]);
  total_error += (deviation[1] - deviation[0]);
  if(deviation[1] == 0)
    return 5;
  else
     correction = kp * deviation[1] + ki * total_error + kd * (deviation[1] - deviation[0]);
     if(correction > 25 || correction < -25) return 25;
     else{
        if(correction<0) return ((int) (-1*correction));
        else return ((int) (correction));
        Serial.print(ki*total_error);
        Serial.print('\t');
  }
}

int check_region(float val1, float val2){
  int dist = (val1 + val2)/2;
  if(dist < dist_to_wall - error)
    return 0;
  if(dist > dist_to_wall + error)
    return 2;
  else return 1;
} 

void lockMotor(int time){
        digitalWrite(dir1,HIGH);
        digitalWrite(dir2,HIGH);
        digitalWrite(dir3,HIGH);
        digitalWrite(dir4,HIGH);
        delay(time);
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
  return dist;
}
