
#include <Servo.h>
#include <QTRSensors.h> 

// initialize servo motors
Servo LeftServo;
Servo RightServo;

// wheel speed constants
// can be adjusted for desired speed - about 90 is zero speed
int count = 0;
int count2 = 0;
int leftGo = 101;
int rightGo = 81;
int leftTurn = 93;
int rightTurn = 86;
int leftReverse = 86;
int rightReverse = 93;
int STOP1 = 93;
int STOP2 = 94;
int RturnL90 = 180;
int RturnR90 = 94;
int intcheckflag = 0;
int ultracheck1 = 0;
int ultracheck2 = 0;
int ultracheck3 = 0;
int time1;

//Ultrasonic sensor pin define
const int trigPin1 = A0;
const int echoPin1 = A1;
const int trigPin2 = A2;
const int echoPin2 = A3;
const int trigPin3 = A4;
const int echoPin3 = A5;

long duration1;
int distance1;
long duration2;
int distance2;
long duration3;
int distance3;
int ultrathred = 25;
int ultrathred1 = 35;
int ultrathred2 = 50;

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3,4,5,6,7,8,10,11},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];



#define TURN_RIGHT 0
#define TURN_LEFT 1
#define FORWARD 2
#define STOP 3
#define BOTHSIDE 4
#define THREESIDE 5
#define FRONTRIGHT 6
#define FRONTLEFT 7
#define FORWARD2 8

bool joint_flag;
unsigned int direction_flag = FORWARD;
unsigned int joint_num = 0;

void LEDON() {
    digitalWrite(9, HIGH);
    delay(500);
    digitalWrite(9, LOW);
}

void LEDblink() {
  if(count > 1){
    digitalWrite(9, HIGH);
    delay(200);
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    delay(200);
    digitalWrite(9, LOW);
  }
}

void Stop(){
  LeftServo.write(90); // Tuned
  RightServo.write(89); // Tuned
  delay(1000);
  }

int joint_check(){//check if the interseciton point found
  if(count  < 1){
    if(sensorValues[0] > 950 && sensorValues[2] > 950 && sensorValues[3] > 950 && sensorValues[5] > 950 && sensorValues[6] > 950 && sensorValues[1] > 950 && sensorValues[4] > 950 && sensorValues[7] > 950)
  
  
  {
    count = count + 1;
    
    return true;
  }
  else{
    return false;
    }
  }
    
  if(sensorValues[0] > 950 && sensorValues[2] > 950 && sensorValues[3] > 950 && sensorValues[5] > 950 && sensorValues[6] > 950)
  {
    count = count + 1;
    if (count > 1){
    LEDON();
    }
    return true;
  }
  else{
    return false;
    }
    
  }
int direction_check(){// check which state is detected by the three sensors
  readdata();
  if((distance1 <= ultrathred1 && distance2 > ultrathred2 && distance3 > ultrathred1) || (distance1 <= ultrathred2 && distance2 > ultrathred1 && distance3 > ultrathred1))  //need to tune
  {
    return TURN_RIGHT;   
  }
  else if((distance2 <= ultrathred1 && distance1 > ultrathred2 && distance3 > ultrathred1) || (distance2 <= ultrathred2 && distance1 > ultrathred2 && distance3 > ultrathred1))  //need to tune
  {
    return TURN_LEFT; 
  }
  else if(distance3 <= ultrathred1 && distance1 > ultrathred1 && distance2 > ultrathred1) //need to tune
  {
    return FORWARD2;  
  }
    else if(distance3 <= ultrathred1 && distance1 <= ultrathred1 && distance2 <= ultrathred1) //need to tune
  {
    return THREESIDE;  
  }
      else if(distance3 > ultrathred1 && distance1 <= ultrathred2 && distance2 <= ultrathred2) //need to tune
  {
    return BOTHSIDE;  
  }
      else if(distance3 <= ultrathred1 && distance1 <= ultrathred1 && distance2 > ultrathred2) //need to tune
  {
    return FRONTRIGHT;  
  }
      else if(distance3 <= ultrathred1 && distance1 > ultrathred2 && distance2 <=  ultrathred1) //need to tune
  {
    return FRONTLEFT;  
  }

  return STOP;
  }



void turnright(){
  LeftServo.write(130);  // need to tune  // Tuned
  RightServo.write(130);   // need to tune  // Tuned
  delay(650);  //need to tune   // Tuned
}
void turnleft(){
  LeftServo.write(57);  // need to tune    // Tuned
  RightServo.write(57);   // need to tune   // Tuned
  delay(700); //need to tune   // Tuned
  }
void forward(){
  LeftServo.write(101);
  RightServo.write(81);
  delay(500);
}
void forward2(){
  LeftServo.write(101);
  RightServo.write(81);
  delay(1000);
}
void right_state(){ //
  bool joint_flag2 = false;
  turnright();
  delay(100);
  followline();
  delay(100);
  readdata(); 



  while(distance3 > 5){ //need to tune the value to see what is the number when it face the object at 5-8cm
    readdata();
    followline();
  }
  
  Stop();
  LEDON();
  
  turnright(); 
  delay(100);
  turnright();
  
  while(joint_flag2 == false){ 
    joint_flag2 = joint_check();
    followline();
  }
  forward();
  turnright(); 
  time1 = millis();
  while(millis()-time1 < 500){ //go forward for 0.5s or sometime in case its not at the right path
    followline();
  }
  joint_flag = false;
  direction_flag = FORWARD;
}
void left_state2(){ //
  bool joint_flag2 = false;
  followline();
  delay(100);
  readdata(); 



  while(distance3 > 4){ //need to tune the value to see what is the number when it face the object at 5-8cm
    readdata();
    followline();
  }
  
  Stop();
  LEDON();
  
  turnright(); 
  delay(100);
  turnright();
  
  while(joint_flag2 == false){ 
    joint_flag2 = joint_check();
    followline();
  }
  forward();
  turnleft(); 
  time1 = millis();
  while(millis()-time1 < 500){ //go forward for 0.5s or sometime in case its not at the right path
    followline();
  }
  joint_flag = false;
  direction_flag = FORWARD;
}
  
void left_state(){ //
  bool joint_flag2 = false;
  turnleft();
  delay(100);
  followline();
  delay(100);
  readdata(); 



  while(distance3 > 4){ //need to tune the value to see what is the number when it face the object at 5-8cm
    readdata();
    followline();
  }
  
  Stop();
  LEDON();
  
  turnright(); 
  delay(100);
  turnright();
  
  while(joint_flag2 == false){ 
    joint_flag2 = joint_check();
    followline();
  }
  forward();
  turnleft(); 
  time1 = millis();
  while(millis()-time1 < 500){ //go forward for 0.5s or sometime in case its not at the right path
    followline();
  }
  joint_flag = false;
  direction_flag = FORWARD;
}

void bothside_state(){
  bool joint_flag2 = false;
  turnright(); 
  delay(100);
  followline();
  delay(100); 
  readdata();

  while(distance3 > 5){ //need to tune the value to see what is the number when it face the object at 5-8cm
    readdata();
    followline();
  }

  LEDON();
  Stop();
  
  turnright();
  delay(100);
  turnright();

  while(joint_flag2 == false){ 
    joint_flag2 = joint_check();
    followline();
  }
 
  left_state2();
 
  }

void forward_state(){
  unsigned int time1 = millis(); // cheat method, go forward for 0.5s
   followline();
  readdata();
  while(distance3 > 5){ //need to tune the value to see what is the number when it face the object at 5-8cm
    readdata();
    followline();
  }
  LEDON();
  while(1){
    Stop(); //
  }
}
void followline(){
unsigned int position = qtrrc.readLine(sensorValues);
  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.println(position); // comment this line out if you are using raw values

  //delay(250);
 // read in sensor values from both photoresistors
 // need to test whether those sensor number is correct or not
 
  if (sensorValues[1] < 100 && sensorValues[4] < 100 ) {  // sees no line on either side, goes straight until a line is seen
    LeftServo.write(leftGo);    // left wheel turns
    RightServo.write(rightGo);  // right wheel turns
  } 
  else if (sensorValues[1] > 900 && sensorValues[0] > 900 && sensorValues[4] < 800) {  // sees line on right side, is making a right turn
    LeftServo.write(leftTurn);      // left wheel turns
    RightServo.write(rightReverse); // right wheel reverses
  } 
  else if (sensorValues[4] > 900 && sensorValues[5] > 900 && sensorValues[1] < 800) {  // sees line on left side, is making a left turn
    LeftServo.write(leftReverse);  // left wheel reverses
    RightServo.write(rightTurn);   // right wheel turns
  } 

  /*else if (sensorValues[1] > 950 && sensorValues[2] > 950 && sensorValues[3] > 950 && sensorValues[4] > 950) { 
    //intcheckflag = intcheckflag++;
    LeftServo.write(STOP1);  // stop
    RightServo.write(STOP2);   // stop
    digitalWrite(9, HIGH);
    delay(500);
    digitalWrite(9, LOW);
    }*/
 
  else {  // both are seeing line, goes straight until light is seen
    LeftServo.write(leftTurn);    // left wheel turns
    RightServo.write(rightTurn);  // right wheel turns
   
  }
  

}

void readfront()
{ digitalWrite(trigPin3, LOW);
    delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin3, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin3, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
    duration3 = pulseIn(echoPin3, HIGH);
  // Calculating the distance
    distance3 = duration3 * 0.034 / 2;
  // Prints the distance on the Seria2 Monitor
    Serial.print("Distance3: ");
    Serial.println(distance3);  
    }

void readdata()//
{     // read ultra 1
  // Clears the trigPin
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
    duration1 = pulseIn(echoPin1, HIGH);
  // Calculating the distance
    distance1 = duration1 * 0.034 / 2;
  // Prints the distance on the Serial Monitor
    Serial.print("Distance1: ");
    Serial.println(distance1); 

  //ultra 2
  // Clears the trigPin
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
    duration2 = pulseIn(echoPin2, HIGH);
  // Calculating the distance
    distance2 = duration2 * 0.034 / 2;
  // Prints the distance on the Serial Monitor
    Serial.print("Distance2: ");
    Serial.println(distance2); 

      //ultra 3
      // Clears the trigPin
    digitalWrite(trigPin3, LOW);
    delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin3, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin3, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
    duration3 = pulseIn(echoPin3, HIGH);
  // Calculating the distance
    distance3 = duration3 * 0.034 / 2;
  // Prints the distance on the Seria2 Monitor
    Serial.print("Distance3: ");
    Serial.println(distance3);  
    }

void setup() {
  delay(500);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
////    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
  
  LeftServo.attach(12);  // attach left servo to pin 6 on arduino
  RightServo.attach(13); // attach right servo to pin 9 on arduino
  digitalWrite(9, LOW);

} 
  
void loop() {

  
  followline(); 
  
  joint_flag = joint_check(); //check if is intersection point
  
  if(joint_flag == true){
    count2 = count2 + 1;
    if (count2 > 1){
    Stop();
    direction_flag = direction_check(); //chech which state it is
    joint_num++;//add joint number, if it is 1 then no need to operate anything
    }

    if(joint_num <= 1){
      joint_flag = false;
    }

    if(joint_num > 1){//go to predesign path
      switch(direction_flag){
        case TURN_RIGHT:LEDblink(); forward(); right_state(); break;
        case TURN_LEFT: LEDblink(); forward(); left_state(); break;
        case FORWARD2: LEDblink();forward_state(); break;
        case BOTHSIDE: LEDblink();forward(); bothside_state(); break;
        case THREESIDE:LEDblink();forward(); bothside_state(); forward_state(); break;
        case FRONTRIGHT: LEDblink();forward(); right_state();forward_state();break;
        case FRONTLEFT: LEDblink();forward(); left_state();forward_state(); break;
      }
      }
    }
}
 

   
   
