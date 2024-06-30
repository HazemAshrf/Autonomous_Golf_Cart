#include <MPU9250_asukiaaa.h>
#include <Arduino.h>

float calculateVelocity(unsigned long &previousMillis);
bool print_imu_data();
void print_data(double timeElapsed);

// Define calibration parameters
float A_a[3][3] = {{0.999121, -0.003824, -0.000976},
                {-0.003824, 0.998942, -0.000044},
                {-0.000976, -0.000044, 0.991294}};
float b_a[3] = {-0.017061, -0.008732, -0.553717};

float A_m[3][3] = {{0.782378, -0.002005, 0.000821},
                {-0.002005, 0.706408, -0.080720},
                {0.000821, -0.080720, 0.716338}};
float b_m[3] = {6.145758, -39.828443, -200.349275};


MPU9250_asukiaaa mySensor;
float aX, aY, aZ, gX, gY, gZ, mX, mY, mZ, mDirection;

const int Pin1 = 2; // Pin for Left wheel
const int Pin2 = 3; // Pin for Right wheel
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousTime = 0;
float velocity1 = 0;
float velocity2 = 0;

void setup() {
  Serial.begin(115200);
  pinMode(Pin1, INPUT);
  pinMode(Pin2, INPUT);
  while (!Serial);

#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#else
  Wire.begin();
  mySensor.setWire(&Wire);
#endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
}

void loop() {
  unsigned long currentTime = millis();

  int sensor_1 = digitalRead(Pin1); //right_sensor
  int sensor_2 = digitalRead(Pin2); //left_sensor
 /* Serial.print(sensor_1);
  Serial.print(",");
  Serial.println(sensor_2);

}*/

  if (sensor_1==HIGH && sensor_2==HIGH){
    previousMillis3 = millis();
    velocity1 = calculateVelocity(previousMillis1);
    while (sensor_1==HIGH){
      unsigned long currentMillis3 = millis();
      unsigned long timeElapsed3 = (currentMillis3 - previousMillis3) / 1000.0;
      if (timeElapsed3 > 0.5){
      velocity1 = 0;
      velocity2 = 0;
      }
      currentTime = millis();
      double timeElapsed = (currentTime - previousTime) / 1000.0;
      previousTime = currentTime;
      print_data(timeElapsed);
      sensor_1 = digitalRead(Pin1);
      sensor_2 = digitalRead(Pin2);
      if (sensor_2==LOW){
        previousMillis3 = millis();
        velocity2 = calculateVelocity(previousMillis2);
        while (sensor_2==LOW){
          unsigned long currentMillis3 = millis();
          unsigned long timeElapsed3 = (currentMillis3 - previousMillis3) / 1000.0;
          if (timeElapsed3 > 0.5){
          velocity1 = 0;
          velocity2 = 0;
          }
          currentTime = millis();
          double timeElapsed = (currentTime - previousTime) / 1000.0;
          previousTime = currentTime;
          print_data(timeElapsed);
          sensor_1 = digitalRead(Pin1);
          sensor_2 = digitalRead(Pin2);
        }
      }
    }
  }

  if (sensor_1==LOW && sensor_2==LOW){
    previousMillis3 = millis();
    velocity2 = calculateVelocity(previousMillis2);
    while (sensor_2==LOW){
      unsigned long currentMillis3 = millis();
      unsigned long timeElapsed3 = (currentMillis3 - previousMillis3) / 1000.0;
      if (timeElapsed3 > 0.5){
      velocity1 = 0;
      velocity2 = 0;
      }
      currentTime = millis();
      double timeElapsed = (currentTime - previousTime) / 1000.0;
      previousTime = currentTime;
      print_data(timeElapsed);
      sensor_1 = digitalRead(Pin1);
      sensor_2 = digitalRead(Pin2);
      if (sensor_1==HIGH){
        previousMillis3 = millis();
        velocity1 = calculateVelocity(previousMillis1);
        while (sensor_1==HIGH){
          unsigned long currentMillis3 = millis();
          unsigned long timeElapsed3 = (currentMillis3 - previousMillis3) / 1000.0;
          if (timeElapsed3 > 0.5){
          velocity1 = 0;
          velocity2 = 0;
          }
          currentTime = millis();
          double timeElapsed = (currentTime - previousTime) / 1000.0;
          previousTime = currentTime;
          print_data(timeElapsed);

          sensor_1 = digitalRead(Pin1);
          sensor_2 = digitalRead(Pin2);
        }
      }
    }
  }

  unsigned long currentMillis3 = millis();
  unsigned long timeElapsed3 = (currentMillis3 - previousMillis3) / 1000.0;
  if (timeElapsed3 > 0.5){
    velocity1 = 0;
    velocity2 = 0;
    
  }
  double timeElapsed = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;
  //delay(10);
  print_data(timeElapsed);
  //delay(100);
  }



  float calculateVelocity(unsigned long &previousMillis) {
  unsigned long currentMillis = millis();
  unsigned long timeElapsed = currentMillis - previousMillis;
  previousMillis = currentMillis;
  
  float circumference = 2 * PI * 22.5; // Circumference of the wheel in cm
  float distance_traveled = circumference / 4; // Distance traveled for each sensor activation
  //float velocity = (distance_traveled / (timeElapsed / 1000.0)) * 0.036; // Convert to km/h
  float velocity = (distance_traveled / (timeElapsed / 1000.0)) / 100;     // Convert to m/s
  return velocity;
}


bool print_imu_data(){
  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    //aSqrt = mySensor.accelSqrt();
    float calibrated_ax = A_a[0][0] * (aX - b_a[0]) + A_a[0][1] * (aY - b_a[1]) + A_a[0][2] * (aZ - b_a[2]);
    float calibrated_ay = A_a[1][0] * (aX - b_a[0]) + A_a[1][1] * (aY - b_a[1]) + A_a[1][2] * (aZ - b_a[2]);
    float calibrated_az = A_a[2][0] * (aX - b_a[0]) + A_a[2][1] * (aY - b_a[1]) + A_a[2][2] * (aZ - b_a[2]);
    Serial.print((calibrated_ax * 9.8) - 0.05);
    Serial.print(",");
    Serial.print((calibrated_ay *9.8));
    Serial.print(",");
    Serial.print(9.81);
    Serial.print(",");
  }

  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    Serial.print(0);
    Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.print(radians(gZ) -0.01);
    Serial.print(",");
  }
    //Serial.print("amr");
  if (mySensor.magUpdate() == 0) {
    //Serial.print("amr");
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    // Apply calibration parameters
    float calibrated_mx = A_m[0][0] * (mX - b_m[0]) + A_m[0][1] * (mY - b_m[1]) + A_m[0][2] * (mZ - b_m[2]);
    float calibrated_my = A_m[1][0] * (mX - b_m[0]) + A_m[1][1] * (mY - b_m[1]) + A_m[1][2] * (mZ - b_m[2]);
    float calibrated_mz = A_m[2][0] * (mX - b_m[0]) + A_m[2][1] * (mY - b_m[1]) + A_m[2][2] * (mZ - b_m[2]);
    mDirection = atan2(calibrated_my, calibrated_mx) * 180 / 3.14;
    if (mDirection < 0){
      mDirection += 360;
    }
    //Serial.print(calibrated_mx);
    //Serial.print(",");
    //Serial.print(calibrated_my);
    //Serial.print(",");
    //Serial.println(calibrated_mz);

    Serial.print(mDirection);
    Serial.print(",");
  }
  return aX >= 0;
}


void print_data(double timeElapsed){
  bool velocity_direction = print_imu_data();
 /* Serial.print(0.00);
  Serial.print(",");
  Serial.print(0.00);
  Serial.print(",");
  Serial.print(0.00);
  Serial.print(",");
  Serial.print(0.00);
  Serial.print(",");
  Serial.print(0.00);
  Serial.print(",");
  Serial.print(0.00);
  Serial.print(",");*/
  
  //if (velocity_direction == 1){
    Serial.print( (velocity1 + velocity2) / 2);
  //}
  //else{
    //Serial.print( (velocity2 + velocity1) / 2);
  //}

  Serial.print(",");
  Serial.println(timeElapsed, 6);
}