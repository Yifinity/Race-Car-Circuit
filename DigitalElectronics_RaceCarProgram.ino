/*
RaceCar Program
This program reads data from buttons/beam breakers and sends digital outputs to the rest of the circuit. 
Program Written for SHS' Digital Electronics Class. 

Written by Yifan Jiang (Class of 2024)
Last Edited: 6/20/2024
*/

// Define pins. 
#define RESET_INPUT 8
#define RESET_OUTPUT 9
#define START_INPUT 2
#define START_OUTPUT 3
#define FINISH1_INPUT 4
#define FINISH1_OUTPUT 5
#define FINISH2_INPUT 6
#define FINISH2_OUTPUT 7

/*
Sensor Class: 
Sensor class takes in an input pin and manages it's respective output pin with a slight delay. 
*/
class Sensor{
  public: 
    int16_t inputPin; 
    int16_t outputPin; 
    unsigned long debounceTime; 
    unsigned long activatedTime; 
    unsigned long targetTime;

    int16_t currentState = 0;
    int16_t previousState = 0;
    bool sensorActivated = false; 

    // Special Variables for reset/start switch
    // Java inheritance doesn't seem to work, so I'm brute forcing it. 
    bool invert; 
    bool isReset; 
    bool dualOutput; 
    int16_t secondOutput; 
  
    Sensor(){}
    Sensor(int16_t input, int16_t output, int16_t debounce){
      // Update Values
      inputPin = input; 
      outputPin = output; 
      debounceTime = debounce; 
      dualOutput = false; 
    }

    // Constructor for dual output switch
    Sensor(int16_t input, int16_t output, int16_t output2, int16_t debounce, bool isToggleable){
      // Update Values
      inputPin = input; 
      outputPin = output; 
      secondOutput = output2; 
      debounceTime = debounce; 
      dualOutput = true; 
      invert = true; 
      isReset = true; 
    }

    void setup(){
      if(dualOutput){
        // Add the second pin as the output
        pinMode(inputPin, INPUT); 
        pinMode(outputPin, OUTPUT); 
        pinMode(secondOutput, OUTPUT); 
        digitalWrite(inputPin, HIGH); 
      }else{
        pinMode(inputPin, INPUT); 
        pinMode(outputPin, OUTPUT); 
        digitalWrite(inputPin, HIGH); 
      }  
    }

    // Method to send out a digital pulse that's registered by the circuit
    void pulse(){
      if(dualOutput){
        // Pulse Reset/Start Pin Accordingly
        if(isReset){
          digitalWrite(outputPin, HIGH); 
          delay(5); 
          digitalWrite(outputPin, LOW); 
          delay(5); 
        }else{
          digitalWrite(secondOutput, HIGH); 
          delay(5); 
          digitalWrite(secondOutput, LOW); 
          delay(5); 
        }
        isReset = !isReset; //Flip isReset so the button alternates between resetting and starting the circuit. 

      }else{
        digitalWrite(outputPin, HIGH); 
        delay(5); 
        digitalWrite(outputPin, LOW); 
        delay(5); 
      }
    }

    // Print information useful for debugging
    void display(){
      Serial.print("PIN ");
      Serial.print(inputPin); 
      Serial.print(" RESULT: ");
      Serial.println(currentState);
    }

    // Method that checks for disturbances and pulses accordingly
    void update(){
      // Update current states
      if(invert){
        currentState = !digitalRead(inputPin); 
      }else{
        currentState = digitalRead(inputPin); 
      }

      if(currentState && !previousState && !sensorActivated){
        // Pulse the sensor
        pulse();
        // Sensor Activated Toggle that holds the sensor from pulsing until delay time is over
        sensorActivated = true; 
        targetTime = millis() + debounceTime; 
      }

      // Check if debounce/delay time is over
      if(sensorActivated){
        activatedTime = millis(); 
        
        // If the time is up, release the boolean blocking any sensor updates.
        if(millis() > targetTime){
          sensorActivated = false; 
        }
      }

      // Update our current state. 
      previousState = currentState; 
    }
};

// Define our sensors
Sensor resetSensor = Sensor(RESET_INPUT, RESET_OUTPUT, START_OUTPUT, 500, true); 
Sensor startSensor = Sensor(START_INPUT, START_OUTPUT, 500);  // Unused - resetSensor doubles as a startSensor
Sensor finish1Sensor = Sensor(FINISH1_INPUT, FINISH1_OUTPUT, 500); 
Sensor finish2Sensor = Sensor(FINISH2_INPUT, FINISH2_OUTPUT, 500); 

// List of sensors
Sensor sensors[] = {resetSensor, startSensor, finish1Sensor, finish2Sensor}; 

// Get size of a list
int16_t numSensors = sizeof(sensors) / sizeof(sensors[0]); // REF: https://forum.arduino.cc/t/solved-getting-array-size/506976
int16_t incomingSelector = -1; 

void setup() {
  Serial.begin(9600); 
  for(byte i = 0; i < numSensors; i++){
    sensors[i].setup(); 
  }

  Serial.println(numSensors);
}

void loop() {
  // Have all the sensors update via polymorphism
  for(byte i = 0; i < numSensors; i++){
    sensors[i].update(); 
  }

  /*
  // Uncomment to read each sensor state
  for(byte i = 0; i < numSensors; i++){
    sensors[i].display(); 
  }
  */

  /*
  //Debug and Test Code
  //Uncommenting this section of code allows you to manually pulse each sensor object by typing 0-3 in the serial monitor
  //Read keyboard input from serial monitor
  if(Serial.available() > 0){
    incomingSelector = Serial.read(); 
  } else{
    // Reset our selector if no valid input or non avaliable
    incomingSelector = -1; 
  }

  // Press 0 to reset circuit
  if(incomingSelector == 48){
    Serial.println("Reseting Testbed");
    resetSensor.pulse();     

  // Press 1 to start circuit
  } else if(incomingSelector == 49){
    Serial.println("Starting Testbed Clock");
    startSensor.pulse();
  
  // Press 2 to toggle Finish1
  } else if(incomingSelector == 50){
    Serial.println("Finish1 Trigger");
    finish1Sensor.pulse();
  
  // Press 3 to toggle Finish2
  } else if(incomingSelector == 51){
    Serial.println("Finish2 Trigger");
    finish2Sensor.pulse();
  }  
  */
}