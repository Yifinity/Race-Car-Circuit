To use this guide, make sure that you have downloaded the Arduino IDE found [here.](https://www.arduino.cc/en/software) 

The main program simply checks each sensor state and sends out a digital signal to the circuit accordingly. Writing such a program allows us to reliably send out signals, add a delay between sensor activation, and systematically test our circuit with controlled tests. 

1. The program is uploaded onto an Arduino Uno using a USB B cable. To make room for the cable, cut the zip ties connecting the battery’s charger and remove the charger. 
>[!WARNING]
>Make sure that the circuit is turned off. 
3. Plug the USB B Cable into the UNO and open up the Arduino IDE. 
4. Download the .ino file in this folder and open it up (File -> Open…). 
5. Once the file is opened, hit the upload button on the top left (Arrow Icon). 
6. Assuming that the USB B is connected, the program should be successfully uploaded. If not, verify that the UNO is plugged in and that you have selected the correct board and port ([Selecting Arduino Board and Port](https://support.arduino.cc/hc/en-us/articles/4406856349970-Select-board-and-port-in-Arduino-IDE)). 
7. With the program uploaded, try running a test and note which sensors do not seem to be triggering. 
8. To isolate a faulty sensor, uncomment the code that’s reserved for debugging (Lines 178-209). 
9. Uncomment the code and run the program. 
10. Open the serial monitor on the top right (Magnifying Glass icon).
>[!IMPORTANT]
>Make sure that you are on 9600 baud ([Using the Serial Monitor Tool](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-monitor/)). 
12. By typing 0-3 and hitting enter, you are able to manually pulse an output for each sensor (Reset, Start, Finish1, Finish2, in that order).  
13. This should be able to isolate which sensor is faulty. Once you do, check the connection from the sensor to the UNO and the connection from the UNO to the port on the PCB. 

