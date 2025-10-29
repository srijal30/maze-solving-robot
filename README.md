# Autonomous Maze Solving Robot

This project was completed for EECE 287 (Computer Organization) Spring 2025.

Bare-metal code for a robot that autonomously navigates a 2D maze. 

Completes the maze by utilizing depth-first search (completely explores the left path first).

For this project, a variety of custom drivers had to be programmed to facilitate interactions with hardware components:

**Custom Drivers for Following Hardware**
- Ultrasonic Sensor
- Seven-segement display
- Motor Driver
- Encoders
- Hardware Timer

Additionally, implemented a basic FSM task scheduler, allowing multiple processes to run simultaneously (motor PID, seven-segment controller, encoder updates, and ultrasonic sensor readings)
