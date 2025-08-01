#include <Servo.h>

// Define pin numbers for input and output
int motionSensorPin = 5;
int servoPin = 2;
int distanceSensorPin = A0;
int in1Pin = 8;
int in2Pin = 9;
int enaPin = 10;

// Define variables
  int motionDetected = 0;
  int distance = 0;
  int motorSpeed = 255;
  int timeVariable = 10;

// Create objects
  Servo myservo;


void setup() {

  Serial.begin(9600);
  // Attach servo to pin
  myservo.attach(servoPin);

  // Set motor control pins to outputs
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enaPin, OUTPUT);

  //Set motion sensor pin to input
  pinMode(motionSensorPin, INPUT);

  // Set initial motor speed to zero
  analogWrite(enaPin, 0);

}

void loop() {

  // Read and print motion sensor input
  int motionDetected = digitalRead(motionSensorPin);
  Serial.println(motionDetected);
  delay (500);

  // Read and print distance sensor input
  distance = analogRead(distanceSensorPin);
  Serial.println(distance);
  delay (500);




  //Run the following if motion is detected by the motion sensor (HIGH)
    if (motionDetected == HIGH) { 
  
      //Make the servo go down, and wait 300 ms before beginning the first motor event
      myservo.write(-135);
      Serial.println("Servo Down");
      delay (300);
      Serial.println("Motor Forward");

      //Run motor forward until the distance is greater than 300, checking distance every 500 ms and storing how much
      //time it has been running
      while (distance < 300){
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
        digitalWrite(enaPin, HIGH);
        distance = analogRead(distanceSensorPin);
        timeVariable = timeVariable + 500;
        delay(500);
        Serial.println(distance);
      }

      //Stop motor for the amount of time that it took to complete the first cluster
      Serial.println("Motor Stop");
      Serial.println(timeVariable);
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, LOW);
      digitalWrite(enaPin, LOW);
      delay(timeVariable);

      //Run motor backwards for the amount of time that it took to complete the first cluster
      Serial.println("Motor Backwards");
      Serial.println(timeVariable);
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, HIGH);
      digitalWrite(enaPin, HIGH);
      delay (timeVariable);

      //reset timevariable for next cycle
      timeVariable = 10;

  //if there is no motion detected, make the servo go up, stop the motor, and wait 500ms before checking the
  //state of the motion sensor again
  } else {
    Serial.println("Servo Up, Motor Stop");    

    myservo.write(135);
  
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    digitalWrite(enaPin, LOW);
    delay (500);

  }
}