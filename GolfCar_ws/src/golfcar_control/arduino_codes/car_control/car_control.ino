#include <PID_v1.h>
#include <Stepper.h>

#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

// Define PID constants
double setpoint = 0;
double input, output;
double kp = 10.0;  // Proportional gain
double ki = 0.1;   // Integral gain
double kd = 0.5;   // Derivative gain

const int velInput = 6;
const int feedbackPin = 3;
const int Relay1 = 5;
const int Relay2 = 4;
const int stepsPerRev = 3200;  //steps per half revolution
const int ForwardPin = 13;
const int BackwardPin = 12;
int signalReceived;
float stepsPerMin = 0.08267717;
int samplingtime = 1;



PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);
Stepper stepper(stepsPerRev, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);


void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(velInput, OUTPUT);
  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
  pinMode(feedbackPin, INPUT);
  analogWrite(ForwardPin, 0);
  analogWrite(BackwardPin, 0);



  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 21);  // Set the output limits (adjust as needed)

  Serial.begin(115200);
  Serial.setTimeout(1);

  //while (Serial.available() < sizeof(int8_t)){
  //delay(900);
  //}
  //Serial.readBytes((char*)&samplingtime, sizeof(int8_t));

  stepper.setSpeed(1);  // Adjust the speed as needed with maximum allowable speed 150 rpm
}

// Read the binary data into variables
float velocity = 0.0;
int32_t steps = 0;
uint8_t brakingSignal = 1;
void loop() {


if (Serial.available() >= sizeof(float) + sizeof(int32_t)) {



  Serial.readBytes((char*)&velocity, sizeof(float));
  Serial.readBytes((char*)&steps, sizeof(int32_t));
  //Serial.readBytes((char*)&brakingSignal, sizeof(uint8_t));


  Serial.print("Velocity: ");
  Serial.println(velocity);
  Serial.print("Steps: ");
  Serial.println(steps);
  //Serial.print("Braking Signal: ");
  //Serial.println(brakingSignal);



  processVelocity(velocity);
  steering(steps);

/*  if (brakingSignal > 0) {
    switch (brakingSignal) {
      case 1:
        brakeInitiation(1000);
        brakePause();
        delay(2000);
        brakeRelease(1000);
        brakePause();
        break;
      case 2:
        brakeInitiation(1750);
        brakePause();
        delay(4000);
        brakeRelease(1500);
        brakePause();
        break;
      default:
        brakePause();
        break;
    }
  }*/
  
  }

  else{
    processVelocity(velocity);
    
  }



}


void steering(int32_t steps) {
  Serial.println(steps);
  int speedo = ceil((stepsPerMin / samplingtime) * abs(steps));  // Convert steps per min to rpm
  stepper.setSpeed(speedo);
  stepper.step(steps);
}



void processVelocity(float velocity) {

  if (velocity > 0) {
    //Serial.println(velocity);
    analogWrite(ForwardPin, 255);




  } else if (velocity < 0) {
    analogWrite(BackwardPin, 255);


    velocity = abs(velocity);


  } else {
    analogWrite(ForwardPin, 0);
    analogWrite(BackwardPin, 0);
  }
  
  int feedbackDuration = pulseIn(feedbackPin, HIGH);

  float feedbackFrequency = 1.0 / (feedbackDuration * 0.000001);
  int CarSpeed = (feedbackFrequency / 20);
  //int CarSpeed = 0;
  //Serial.println(CarSpeed);
  //Serial.println(velocity);


  input = CarSpeed;
  setpoint = velocity;

  myPID.Compute();  // Calculate the PID output

  // Map the output value to the 0-255 range for analogWrite
  int mappedOutput = map(output, 0, 21, 60, 255);
  analogWrite(velInput, mappedOutput);

  delay(5);
}

/*
void brakeInitiation(int duration) {
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, HIGH);
  stopMotor();
  delay(duration);
}

void brakeRelease(int duration) {
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, LOW);
  stopMotor();
  delay(duration);
}

void brakePause() {
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
}

void stopMotor() {
  delay(500);
  analogWrite(velInput, 0);
  delay(500);
}*/
