
# Arduino-Obstacle-Avoiding-Robot-controlled-by-voice

This project represents hardware and software design and the development of a robot controlled by voice. The idea of this project is the realization of a mobile robot that would have the ability to control via voice as well as a remote control with the additional ability to detect obstacles and bypass them. Control would be realized through the relevant Bluetooth module, using a mobile phone and application. A motor driver is used to control the motors themselves. Obstacle detection is provided by an ultrasonic sensor. The idea is for this robot to be intelligent enough to obey certain commands, move around space and detect obstacles as well as avoiding them.

# Robot display
Check out the working of the robot in the video by clicking on the picture below:

[![Arduino-Obstacle-Avoiding-Robot-controlled-by-voice](https://user-images.githubusercontent.com/75175755/111798156-1579b080-88ca-11eb-9cc9-701813a57c7a.jpg)](https://www.youtube.com/watch?v=0ZskeoQ-Kg4)

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

If in this state obstacle is detected robot will start execution of obstacle avoiding algorithm. 

# Obstacle avoiding algorithm

This robot provides avoiding several types of obstacles. His main goal is to follow the initial route while avoiding the obstacle. He follows one edge of the obstacle with an ultrasonic sensor. When the robot realizes that he passed one side of the obstacle, he turns to a certain side which he thinks is better and starts to follow and avoid that side of the obstacle. When that side is passed he considers that the obstacle has been passed and returns to the old trajectory. 

This robot has the ability to:

1. If an obstacle is limited and the robot can go on the left or right side, he will avoid that obstacle
2. If the robot gets new command while avoiding an obstacle, the command will have a higher priority
3. Robot can detect tunnel-type obstacles, in that case, he stops and waits for the new command
4. Robot can detect walls
5. Robot can avoid more obstacles during the same trajectory


For more details, you can check my thesis report in the files above.
