
int mode = 0 ;   /// initial mode- bluetooth controlled
int obstacle = 6;   // LED indicating obstacle avoider mode
int line_follow = 7 ;  // LED indicating line follower mode
// ------------global variables for obstacle

const int trigPin = 9;
const int echoPin = 8;
long duration;
int distance;
int speeds = 220;
//-------------global variables for robot control

const int right_motor_direction = 2;
const int right_motor_speed = 3;

const int left_motor_direction = 4;
const int left_motor_speed = 5;
//------------- for bluetooth control
int input = 0;  //  received data from remote via bluetooth
//---------------------------------------------------------------------------------------------------------------------------



void setup() {
  pinMode(obstacle, OUTPUT);
  pinMode(line_follow, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(right_motor_direction, 1);
  pinMode(left_motor_direction, 1);
  Serial.begin(9600); // Starts the serial communication
}
void loop() {

  if
  (Serial.available() > 0)
  {
    input = Serial.read();
    // Serial.println(input);
    if (input == 116) {        //   if "Turbo" in the android app. is pressed mode will be changed
      mode = mode + 1 ;
      if (mode == 3) mode = 0;
      delay(300) ;
    }
  }

  ///////////////////////////////////
  if (mode == 0) {             // this is BL controlled car mode where two indicating LEDs are off
    digitalWrite(obstacle, 0);
    digitalWrite(line_follow, 0);
    switch (input) {
      case 108:    /////////////////////////Left
        left();
        break;
      case 114:    ///////////////////////// Right
        right();
        break;
      case 117:    ///////////////////////// Up i.e. Front
        front();
        break;
      case 100:    ///////////////////////// Down i.e. back
        back();
        break;
      default:       //////////////////// stop
        dead_stop();
        break;
    }
  }

  else if (mode == 1) obstacle_avoider(); // this is Obstacle avoider mode where only left side indicating LED is on connected to port 6
  else if (mode == 2) line_follower();    // this is line follower mode where only right side indicating LED is on connected to port 7

}

void front()
{ digitalWrite(right_motor_direction, 0); //  0 means forward direction
  analogWrite(right_motor_speed, speeds);    //  adjust speed ; 0 is lowest and 255 is highest for forward direction
  digitalWrite(left_motor_direction, 0);
  analogWrite(left_motor_speed, speeds);
}
void left()
{ digitalWrite(right_motor_direction, 0);
  analogWrite(right_motor_speed, speeds);
  digitalWrite(left_motor_direction, 1); //  1 means reverse direction
  analogWrite(left_motor_speed, 0);     //  adjust speed ; 255 is lowest and 0 is highest for reverse direction
}
void right()
{ digitalWrite(right_motor_direction, 1);
  analogWrite(right_motor_speed, 0);
  digitalWrite(left_motor_direction, 0);
  analogWrite(left_motor_speed, speeds);
}
void back()
{ digitalWrite(right_motor_direction, 1);
  analogWrite(right_motor_speed, 0);
  digitalWrite(left_motor_direction, 1);
  analogWrite(left_motor_speed, 0);
}
void dead_stop()
{ digitalWrite(right_motor_direction, 0);
  analogWrite(right_motor_speed, 0);
  digitalWrite(left_motor_direction, 0);
  analogWrite(left_motor_speed, 0);
}


void obstacle_avoider()
{
  digitalWrite(obstacle, 1);
  digitalWrite(line_follow, 0);
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // in cm
  // Prints the distance on the Serial Monitor
  Serial.print("Distance in cm.: ");//////////
  Serial.println(distance);/////////////
  obstacle_avoid();
}



void obstacle_avoid()
{

  if (distance > 7) {               //  you can change this value to adjust minimum distance between robot and obstacle to sense
    front();
  }

  else if (distance < 8 && distance > 4) {
    left();
    delay(500);              //  you can change this value to adjust angle of rotation in left side
  }
  else if (distance < 5) {
    back();
    delay(500);             //  you can change this value
  }
}
////////////////////////////////////////////////--------------------------------
void line_follower()
{
  digitalWrite(obstacle, 0);
  digitalWrite(line_follow, 1);
  int sensorValue_1 = analogRead(A1);
  int sensorValue_2 = analogRead(A2);
  int sensorValue_3 = analogRead(A3);
  int th_1 = 850, th_2 = 200, th_3 = 850; // threshold values adjusted after observation through serial monitor
  Serial.print(sensorValue_1);
  Serial.print("\t");
  Serial.print(sensorValue_2);
  Serial.print("\t");
  Serial.print(sensorValue_3);
  Serial.print("\n");
  
  if (sensorValue_1>sensorValue_2 && sensorValue_2>sensorValue_3 && sensorValue_1>sensorValue_3)
  {
    right();
  }
   if (sensorValue_1>sensorValue_2 && sensorValue_1>sensorValue_3)
  {
    right();
  }
   if (sensorValue_3>sensorValue_1 && sensorValue_3>sensorValue_2 && sensorValue_2>sensorValue_1)
  {
    left();
  }
   if (sensorValue_3>sensorValue_1 && sensorValue_3>sensorValue_2 )
  {
    left();
  }
   if (sensorValue_1 < 200 && sensorValue_2 < 200 && sensorValue_3 < 200)
  {
    front();
  }
   if (sensorValue_1 < 300 && sensorValue_2 < 300 )
  {
    front();
  }
   if ( sensorValue_2 < 300 && sensorValue_3 < 300)
  {
    front();
  }
  
   if (sensorValue_1 > 850 && sensorValue_2 >850 && sensorValue_3 > 850)
  {
    dead_stop();
  }
  
  
/*
  if (sensorValue_1 > th_1 && sensorValue_2 < th_2 && sensorValue_3 > th_3)
  {
    front();
  }
  else if (sensorValue_1 < th_1 && sensorValue_2 > th_2 && sensorValue_3 > th_3)
  {
    right();
  }
  else if (sensorValue_1 < th_1 && sensorValue_2 < th_2 && sensorValue_3 > th_3)
  {
    right();
  }
  else if (sensorValue_1 > th_1 && sensorValue_2 > th_2 && sensorValue_3 < th_3 )
  {
    left();
  }
  else if (sensorValue_1 > th_1 && sensorValue_2 < th_2 && sensorValue_3 < th_3 )
  {
    left();
  }
  else if (sensorValue_1 > th_1 && sensorValue_2 > th_2 && sensorValue_3 > th_3)
  {
    dead_stop();
  }
  else if (sensorValue_1 < th_1 && sensorValue_2 < th_2 && sensorValue_3 < th_3)
  {
    dead_stop();
  }*/
  // you can add even more conditions as your wish for improvement
}




