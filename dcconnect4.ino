
#include <Servo.h>
#define STEPS 200
char val='i';
int pos=0;
int col;
Servo myservo;
int motorPin1 =  6;    // One motor wire connected to digital pin 5
int motorPin2 =  3;    // One motor wire connected to digital pin 6

int distance(char num);

void setup()
{
  Serial.begin(9600);

  myservo.attach(5);
  pinMode(motorPin1, OUTPUT); 
  pinMode(motorPin2, OUTPUT);  
}

void loop()
{
  myservo.write(0);
  if(Serial.available()){
    
    val = Serial.read();
   
  }
  else
  {
    if(val!='i')
    {
      Serial.println(forward(val));
          rotateRight(195,forward(val));
        
          delay(1000);
          for(pos = 0; pos < 90; pos += 1)   
            {                                  // in steps of 1 degree 
              myservo.write(pos);              // tell servo to go to position in variable 'pos' 
              Serial.print("Servo");
              Serial.println(pos);
              delay(15);                       // waits 15ms for the servo to reach the position 
              } 
          for(pos = 90; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
            {                                
              myservo.write(pos);              // tell servo to go to position in variable 'pos' 
              Serial.print("Servo");
              Serial.println(pos);
              delay(15);                       // waits 15ms for the servo to reach the position 
              } 
          delay(1000);

          rotateLeft(180,backward(val));
          val ='i';
        
      }
      
    }
    
  }
  


int forward(char num)
{
  col = num - '0';
  switch(col)
  {
    case 1: return 160;
    case 2: return 330;
    case 3: return 480;
    case 4: return 640;
    case 5: return 800;
    case 6: return 960;
    case 7: return 1120;
    default: return 0;
  }
}
int backward(char num)
{
  col = num - '0';
  switch(col)
  {
    case 1: return 170;
    case 2: return 340;
    case 3: return 480;
    case 4: return 640;
    case 5: return 800;
    case 6: return 960;
    case 7: return 1120;
    default: return 0;
  }
}
void rotateLeft(int speedOfRotate, int length){
  analogWrite(motorPin1, speedOfRotate); //rotates motor
  digitalWrite(motorPin2, LOW);    // set the Pin motorPin2 LOW
  delay(length); //waits
  digitalWrite(motorPin1, LOW);    // set the Pin motorPin1 LOW
}

void rotateRight(int speedOfRotate, int length){
  analogWrite(motorPin2, speedOfRotate); //rotates motor
  digitalWrite(motorPin1, LOW);    // set the Pin motorPin1 LOW
  delay(length); //waits
  digitalWrite(motorPin2, LOW);    // set the Pin motorPin2 LOW
}

void rotateLeftFull(int length){
  digitalWrite(motorPin1, HIGH); //rotates motor
  digitalWrite(motorPin2, LOW);    // set the Pin motorPin2 LOW
  delay(length); //waits
  digitalWrite(motorPin1, LOW);    // set the Pin motorPin1 LOW
}

void rotateRightFull(int length){
  digitalWrite(motorPin2, HIGH); //rotates motor
  digitalWrite(motorPin1, LOW);    // set the Pin motorPin1 LOW
  delay(length); //waits
  digitalWrite(motorPin2, LOW);    // set the Pin motorPin2 LOW
}
