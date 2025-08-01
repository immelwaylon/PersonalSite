---
title: "Automatic Door Opening System"
summary: "Collaborated with one of my peers to design and develop a sensor/arduino controlled automatic door system, reducing costs 90% compared to industry standards."
categories: ["Projects"]
tags: ["Programming","Engineering", "School"]
#externalUrl: ""
#showSummary: true
date: 2023-09-22
draft: false
---

<video controls width="100%">
  <source src="/media/PLTW Recording.mp4" type="video/mp4">
  Your browser doesn't support HTML5 video.
</video>

## Extended Summary
This was a high school project for the [PLTW](https://en.wikipedia.org/wiki/Project_Lead_the_Way) course, "Engineering Design and Development". My peer, Aaron Kosoff, and I went through the entire design and development process. We started by researching a problem, examining its current solutions, and deciding on a final innovation that could improve upon current solutions. We landed on an adhesive-mounted door opening system controlled by various sensors and an arduino, as it would reduce costs by a few orders of magnitude while preventing 100% of pathogen spread. Through extensive prototyping, we landed on an acceptable rough design that met our design requirements.

## Research
#### Problems
The design process began with researching problems. We had a few different ideas, but settled on pathogen spread through door handles because we had found a good amount of research showing that it was a significant problem. 
#### Previous Solutions
This led to us looking into previous solutions and comparing them in a table. The graded overall best solutions we found were automatic doors and a door stop, since they prevent contact with the door completely. The door stop's main problem was that it removes the security and insulation that a door provides, while the automatic door was found to be overly expensive ($10k+). Because of this, we knew we had to create something that made doors touch-free at a low cost. 
#### Brainstorming
We came up with a number of possible solutions to the problem. One solution was an automatic door that could be mounted above an empty doorframe, behaving like a garage door. We decided this would be too impractical for use. Another idea was a door handle that heated up to kill bacteria. This idea was shot down because of obvious safety concerns. Finally, we came to the idea of a system that you could mount to any door to make it an automatic door. This idea seemed affordable, safe, and preserved the insulation and security of the original door. 

## Electronics Prototyping
After our initial sketches of the system, we came up with an electrical system that would work to open the most doors. We determined that we would need a powerful motor to open the door (we calculated the force and leverage but i don't have access to my old notes right now), a servo to turn the handle to a specified angle, a motion sensor to detect movement, and a distance sensor to keep the system from hitting people and walls while opening the door.

## Mechanics Prototyping
Using our initial sketches, we made a rough prototype to test the feasability of pulling the whole door with adhesive strips and a small motor. This confirmed that our mounting solution was valid, and that the motor was powerful enough when given the correct voltage.

Our second prototype was 3d printed and used an aluminum wheel with a rubber grip so that the shaft wouldn't shred the inside of the wheel, and the wheel could grip the ground. This didn't work so well because the wheel didn't have enough traction. We calculated the force we would need to put on the ground given the friction coefficient of the wheel. Instead of considering this, we decided to test various springs to impart ground force, using the one that worked most effectively. 

After that, we went through various mechanical prototypes. Between these prototypes, we added/adjusted mounting points for the various sensors. We also slowly built in spaces for the arduino, motor controller, and battery to fit. Eventually we came to a final design that had mounting points for everything and a shroud to cover all of the internal wiring. This design was functional, but not exactly production ready as the shroud was simply slipped on, and not locked on like a public facility would need.

Below shows, in order, a front view of mount with components, a top right view of mount structure, and a top right view of mount with components. Shroud model was excluded from these images.

<div style="display: flex; gap: 10px;">
  <img src="/media/PLTW/Full%20Thing/Front.png" alt="Front" style="width: 49%;">
  <img src="/media/PLTW/Full%20Thing/TopRight.png" alt="Top Left" style="width: 49%;">  
  <img src="/media/PLTW/Full%20Thing/Top%20Right.png" alt="Top Left" style="width: 49%;">
</div>

## Code Prototyping
We started with a basic pseudocode framework that looked something like this:

    Loop forever{
        If motion sensor activated:
            Turn door handle servo
            Motor forwards
            Loop until distance sensor is too close{
                Wait a short time
                }
            Wait for someone to walk through the door
            Motor backwards for the time it took to open
            Return Door handle servo
        Else:  
        Wait a short time
    }

We researched how to use C++ to recieve and send signals through an Arduino. Through a few iterations, and using the pseudocode as a guide, we eventually developed the following code.

<details>
<summary>Code</summary>

```c++
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

```
</details>

## Presentation
Finally, we presented our project at a public school exhibition with industry experts that were asked to come rate us. Our teacher, Ms. Mason, then asked us and one other group whether we'd like to present our projects to the Cotati-Rohnert Park District School Board to show how Engineering Design and Development was a worthwhile course to continue teaching in the school district. We accepted, and presented our project there as well.

## Reflection
In conclusion, this was a fun and successful project. I thoroughly enjoyed the design process and getting to make something that could actually benefit people in the real world. This project exercised and improved my teamwork, presentation, CAD, coding, and electrical systems design skills all while allowing me to make something, which was really awesome.