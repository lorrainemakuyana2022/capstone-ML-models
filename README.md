# Capstone - ML Models for Arduino NANO BLE Sense and FRDM-K64F

For my final year Capstone Project, I developed a machine learning model for motion detection to detect motion in these 5 classes: 
* idle 
* clockwise
* anticlockwise
* left-right
* up-down 

This repository serves as the collection of my Capstone Project Code and files that a person may need if they are to reproduce my project on the same hardware I used. 

## For Arduino Nano BLE Sense development board
For development on the Arduino Nano, you can download the .zip library in the arduino_nano_ble folder in this repo with the name **ei-motion-detection-final-arduino-1.0.11.zip** and store the library in a known location. Also, to see different configurations and data samples used to come up with this NN, head over to my [Edge Impulse Project](https://studio.edgeimpulse.com/public/87639/latest). Follow the next few steps to download an Arduino .zip Library to add to your IDE and to test the project. It is assumed that you already have Arduino IDE installed and you also have an Arduino Nano 33 BLE Sense board. If not, you can download the IDE [here](https://www.arduino.cc/en/software) and purchase an Arduino Nano 33 BLE Sense board at an electronics shop near you.

#### Steps
1. Download the .zip library referenced above from this repository. Save this in a known location 
2. Head over to your Arduino IDE
3. Go to Sketch -> Include Library -> Add .ZIP library and select the zip file you just downloaded 
4. Go to File -> Examples -> motion-detection-final_inferencing -> nano_ble33_sense_accelerometer_continuous
5. Plug in your Arduino Nano 33 BLE Sense Board to your computer and ensure the right Port and Board are selected under the Tools menu
6. Upload the sketch 
7. Open the Serial Monitor to see live classification when moving the board in the desired motion. **NB** you may need to re-check the Port after you upload as this may have changed.
8. To see the time for inferencing calculated and used for this project, download the **arduino_nano_final_test.ino** file in this repository and run it in your Arduino IDE. 

## For FRDM-K64F development board
For development on thee FRDM-K64F development board, it is assumed you already have a FRDM-K64F development board, MCUExpresso IDE and that you have already installed the SDK for the K64F board to enable board support in MCUExpresso. If you do not have these, head to NXP's website to download and install the software and to an electronics shop near you to get a K64F development board. 

#### Steps 
1. Create a C++ application in MCUExpresso IDE. When prompted, choose C++ project and Semihost Console to enable you to see inferencing results. 
2. From the frdm-k64f folder of this repository, download the **library** folder and extract it. Place the folders ei, mp and tflite in the **source** folder in the project you just created.
3. Copy the contents of the <code>main.cpp</main> file into the <code>Project_Name.cpp</code> in your project source folder. 
4. Connect your FRDM-K64F board to your PC. Build and Run the project in Debug Mode to be able to see inferencing results.
