---
title: "Automatic Door Opening System"
summary: "Designed and developed an Arduino-based automatic door system with sensor integration, achieving 90% cost reduction compared to commercial solutions."
categories: ["Projects"]
tags: ["Programming", "Engineering", "School"]
date: 2023-09-22
draft: false
---

<video controls width="100%">
  <source src="/PersonalSite/media/PLTW_Recording.mp4" type="video/mp4">
  Your browser doesn't support HTML5 video.
</video>

## Introduction
As part of the [Project Lead The Way](https://en.wikipedia.org/wiki/Project_Lead_the_Way) "Engineering Design and Development" course, my partner Aaron Kosoff and I were challenged to identify a real-world problem and develop an innovative engineering solution. We designed an adhesive-mounted, sensor-controlled automatic door system that reduces costs by 90% compared to traditional automatic doors while preventing pathogen transmission through touch-free operation.

## Research & Problem Identification
### Problem Space
We began by researching significant everyday problems that lacked cost-effective solutions. Pathogen transmission through door handles emerged as a critical issue, with studies showing high contamination rates on frequently touched surfaces.

### Existing Solutions Analysis
We evaluated current solutions through comparative analysis:
- **Automatic doors**: Effective but prohibitively expensive ($10,000+)
- **Door stops**: Eliminate contact but compromise security and insulation
- **Antimicrobial handles**: Reduce but don't eliminate transmission risks

This analysis revealed a clear need for a touch-free solution that maintained door functionality while being economically viable.

### Concept Development
After brainstorming multiple approaches, we converged on an adhesive-mounted automation system that could retrofit existing doors. This solution preserved security and insulation while enabling touch-free operation at a fraction of conventional automatic door costs.

## Technical Development
### Electronic System Design
The system architecture integrated multiple components:
- **High-torque motor** for door operation (calculated torque requirements based on door weight and leverage)
- **Servo mechanism** for handle rotation
- **Motion sensor** for user detection
- **Ultrasonic distance sensor** for safety collision avoidance
- **Arduino Uno** for system control and integration

### Mechanical Prototyping
We progressed through multiple prototyping iterations:

**Initial Proof-of-Concept**
Validated adhesive mounting viability and motor torque requirements through rough physical testing. Determined 3D Printed wheels were unfeasable.

**Traction System Development**
Switched to commercial aluminum wheels with rubber grips, then implemented spring-based force adjustment to optimize ground traction.

**Mechanical Optimization**
To ensure the mounting system could withstand repeated operational stresses, I used Fusion 360's static stress simulation to validate and refine the design.
* **Simulation Parameters**: Applied a 9.81 N (1 kgf) load upward on the motor mount to simulate the reactive force generated when the wheel pulls against the floor to open a door.
* **Analysis**: The simulation identified high-stress concentration points, visualized in red, which indicated potential areas for failure under load.
* **Iterative Design**: Based on the results, I added strategic fillets and reinforced thin cross-sections in the 3D model. This process significantly increased the factor of safety without adding excessive material or weight, ensuring long-term durability.
<div style="display: flex; gap: 10px; flex-wrap: wrap;">
  <img src="/PersonalSite/media/PLTW/FullThing/Sim1_9.81N_Upwards_Front.png" alt="Simulation" style="width: 32%;">
</div>

**Final Assembly Integration**
Developed a comprehensive housing solution with:
- Mounting points for all electronic components
- Integrated wiring management
- Removable shroud for maintenance access
- Structural optimization for weight distribution

<div style="display: flex; gap: 10px; flex-wrap: wrap;">
  <img src="/PersonalSite/media/PLTW/FullThing/Front.png" alt="Front view with components" style="width: 32%;">
  <img src="/PersonalSite/media/PLTW/FullThing/TopRight.png" alt="Top-right structural view" style="width: 32%;">  
  <img src="/PersonalSite/media/PLTW/FullThing/Top_Right.png" alt="Top-right view with components" style="width: 32%;">
</div>

## Code Prototyping
We started with a basic pseudocode framework:

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
<summary>Click to expand final code</summary>

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

## Results and Recognition
### Performance Achievements
* **90% cost reduction** compared to commercial automatic doors
* **100% pathogen transmission prevention** through touch-free operation
* **Successful operation** on standard interior doors
* **Quick installation** via adhesive mounting system

### Performance Achievements
We demonstrated our system at multiple venues:
1. **School Exhibition**: Presented to industry experts and received top marks for innovation and practicality
2. **School Board Presentation**: Selected as one of two groups to demonstrate the value of engineering education to the Cotati-Rohnert Park Unified School District

## Reflection
This project provided comprehensive engineering experience across multiple disciplines:
* Systems integration combining mechanical, electrical, and software components
* Iterative design through multiple prototyping cycles
* Cost-benefit analysis and practical constraint consideration
* Technical communication through public presentations

The project successfully demonstrated how engineering innovation can create accessible solutions to everyday problems while developing valuable skills in CAD, programming, electronics, and collaborative design.