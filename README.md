# Arduino-Obstacle-Avoiding-Robot-controlled-by-voice

This project represents hardware and software design and the development of a robot controlled by voice. The idea of this project is the realization of a mobile robot that would have the ability to control via voice as well as a remote control with the additional ability to detect obstacles and bypass them. Control would be realized through the relevant Bluetooth module, using a mobile phone and application. A motor driver is used to control the motors themselves. Obstacle detection is provided by an ultrasonic sensor. The idea is for this robot to be intelligent enough to obey certain commands, move around space and detect obstacles as well as avoiding them.

# Robot display


![prikazrobota](https://user-images.githubusercontent.com/75175755/111798156-1579b080-88ca-11eb-9cc9-701813a57c7a.jpg)


# Hardware realization
List of hardware components used:

1. Arduino UNO R3
2. L293D Motor Drive Shield
3. 4x Wheel and Gear Motor
4. SG - 90 Servo Motor
5. HC-SR04 Ultrasonic Distance Module 
6. HC-05 Bluetooth module

The first step was wiring and connecting all components. The picture below shows how everything is wired together.

![povezivanje](https://user-images.githubusercontent.com/75175755/111800169-fed45900-88cb-11eb-9d23-45f4ef7fe03c.png)


# Software realization

The software realization has two main parts:

1. Design and development of Android application using MIT App Inventor
2. Design and development of Arduino application using Arduino IDE

The appearance of the android application

![aplikacija](https://user-images.githubusercontent.com/75175755/111801003-cc772b80-88cc-11eb-8d1f-d12db349570a.png)


# Robot control

A robot has two types of control: remote and voice control.
Possible remote commands are:

1. Forward
2. Backward
3. Stop
4. Left 
5. Right

In this type of control if the robot detects an obstacle he stops.

The second type of control is voice control.
Possible voice commands are:

1. "Kreni"(Start)
2. "Stani"(Stop)
3. "Naprijed"(Forward)
4. "Nazad"(Backward)
5. "Lijevo"(Left)
6. "Desno"(Right)
7. "Brže"(Faster)
8. "Sporije"(Slower)

If in this state obstacle is detected robot will start execution obstacle avoiding algorithm. 

# Obstacle avoiding algorthm

This robot provides avoiding servel types of obstacles. He attempts to avoid that obstacle by following obstacle's edge and detect when 
