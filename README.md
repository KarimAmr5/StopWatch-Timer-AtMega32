
# StopWatch-Timer-AtMega32

> [!NOTE]
>This project was made during the Full Embedded Systems Diploma under the supervision of Engineer Mohamed Tarek.

This project purpose is to implement a StopWatch timer system using the following format [HH:MM:SS] with the ability to pause , stop or reset the timer . It uses ATMEGA32 microcontroller along with 6x 7-segment-display to display the clock .


## Table of contents 

- [Overview](#Overview)
- [Requirements](#Requirements)
- [Components](#Components)
- [Circuit](#Circuit)
- [How to run ](#How-to-run)
- [Running Video](#Running-Video)


## Overview

In this project , the main aim is to figure out how to display the the six digits of the clock and refreshing them all at once ! 
It simply uses technique of fast refreshing on all digits one by one at a very high speed that makes it looks they're all displaying at the same time , will updating the value of each digit whenever needed . At speed of 1Mhz it will be very convenient to make it look authentic and does the job seamlessly4 !

## Requirements

Follow the fully detailed project requiremnts in the following file : [Project requirements file](https://github.com/KarimAmr5/StopWatch-Timer-AtMega32/blob/main/Mini%20Project%202%20Req.pdf)


## Components

- ATmega32 Microcontrollers running @ 1 MHz
- 6x 7-Segment-display
- 3x Push Buttons
- 6x Transistors 
- 1x 7-Segment-Decoder

## Circuit 

![circuit](https://github.com/user-attachments/assets/0bd10be7-2f51-47d4-bb27-d21bb02f9f8c)


## How to run

> The purpose of this project is run on Proteus simulation software , but it could still run on real AtMega32 Microcontrollers with small changes to the code , for instace the frequenct (set to 16 Mhz) , etc.

**Required software for simulation run :** 
- Install Proteus 8 professional v10.0 or higher
- Install required Arduino & AVR libraries (Arduino_Proteus_Libraries)
- Run the .pdsprj file !

**Application procedure**

- Start the StopWatch Timer 
- You can try to stop , reset or pause and check the accuracy of the timer !

**Running Video**


https://github.com/user-attachments/assets/f40dbb3f-d46e-4b2c-adb4-15b538cddf5c


