# Capstone - ML Models for Different Microcontrollers and Development Boards

For my final year Capstone Project, I developed a machine learning model for motion detection to detect motion in these 5 classes: 
* idle 
* clockwise
* anticlockwise
* left-right
* up-down 

This repository serves as the collection of my Capstone Project Code and files that a person may need if they are to reproduce my project on the same hardware I used. 

## For Arduino Nano BLE Sense development board
For development on the Arduino Nano, head over to my [Edge Impulse Project](https://studio.edgeimpulse.com/public/87639/latest) and follow the next few steps to download an Arduino .zip Library to add to your IDE. It is assumed that you already have Arduino IDE installed and you also have an Arduino Nano 33 BLE Sense board. If not, you can download the IDE [here](https://www.arduino.cc/en/software) and purchase an Arduino Nano 33 board at any electronics shop near you.

#### Steps
1. Click on Deployment in the left menu
2. Select Arduino Library 
3. Scroll down, at the **Select Optimizations** panel, choose Quantized (int8) 
4. Click build (This will build and download a .zip file for the project for you. Save this in a known location 
5. Head over to your Arduino IDE
6. Go to Sketch -> Include Library -> Add .ZIP library and select the zip file you just downloaded 
7. Go to File -> Examples -> motion-detection-final_inferencing -> nano_ble33_sense_accelerometer_continuous
8. Plug in your Arduino Nano 33 BLE Sense Board to your computer and ensure the right Port and Board are selected under the Tools menu
9. Upload the sketch 
10. Open the Serial Monitor to see live classification when moving the board in the desired motion. **NB** you may need to re-check the Port after you upload as this may have changed.
