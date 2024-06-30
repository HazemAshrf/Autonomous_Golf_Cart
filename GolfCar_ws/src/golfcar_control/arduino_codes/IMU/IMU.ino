#include <MPU9250_asukiaaa.h>
#include <Adafruit_BMP280.h>

// Define calibration parameters
float A_m[3][3] = {{0.782378, -0.002005, 0.000821},
                {-0.002005, 0.706408, -0.080720},
                {0.000821, -0.080720, 0.716338}};
float b_m[3] = {6.145758, -39.828443, -200.349275};


float A_a[3][3] = {{0.999121, -0.003824, -0.000976},
                {-0.003824, 0.998942, -0.000044},
                {-0.000976, -0.000044, 0.991294}};
float b_a[3] = {-0.017061, -0.008732, -0.553717};


MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;

void setup() {
  Serial.begin(115200);
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

  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    //aSqrt = mySensor.accelSqrt();
    float calibrated_ax = A_a[0][0] * (aX - b_a[0]) + A_a[0][1] * (aY - b_a[1]) + A_a[0][2] * (aZ - b_a[2]);
    float calibrated_ay = A_a[1][0] * (aX - b_a[0]) + A_a[1][1] * (aY - b_a[1]) + A_a[1][2] * (aZ - b_a[2]);
    float calibrated_az = A_a[2][0] * (aX - b_a[0]) + A_a[2][1] * (aY - b_a[1]) + A_a[2][2] * (aZ - b_a[2]);
    Serial.print((calibrated_ax * 9.8) + 0.1725);
    Serial.print(",");
    Serial.print((calibrated_ay *9.8) + 0.045);
    Serial.print(",");
    Serial.println(calibrated_az * 9.8);
    //Serial.print("\taccelSqrt: " + String(aSqrt));
  }

  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    Serial.print(radians(gX)+ 0.004623);
    Serial.print(",");
    Serial.print(radians(gY) - 0.016054);
    Serial.print(",");
    Serial.println(radians(gZ)- 0.012767);
  }

  if (mySensor.magUpdate() == 0) {
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
    Serial.print(calibrated_mx, 6);
    Serial.print(",");
    Serial.print(calibrated_my, 6);
    Serial.print(",");
    Serial.println(calibrated_mz, 6);
    Serial.println("thorizontalDirection: " + String(mDirection));
  }
  delay(100);
  }


//position
/*
#include <MPU9250_asukiaaa.h>
#include <Adafruit_BMP280.h>

// Define calibration parameters
float A_m[3][3] = {{0.634985, 0.004124, -0.002058},
                    {0.004124, 0.639214, 0.001597},
                    {-0.002058, 0.001597, 0.665433}};
float b_m[3] = {-9.550697, 56.895877, -13.634070};

float A_a[3][3] = {{0.999121, -0.003824, -0.000976},
                    {-0.003824, 0.998942, -0.000044},
                    {-0.000976, -0.000044, 0.991294}};
float b_a[3] = {-0.017061, -0.008732, -0.553717};

MPU9250_asukiaaa mySensor;
float aX, aY, aZ, gX, gY, gZ, mX, mY, mZ;

float velocityX = 0; // Initial velocity in x direction
float positionX = 0; // Initial position in x direction

unsigned long prevTime = 0;

void setup() {
    Serial.begin(115200);
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
    float dt = (currentTime - prevTime) / 1000.0; // Convert milliseconds to seconds

    if (mySensor.accelUpdate() == 0) {
        aX = mySensor.accelX();
        aY = mySensor.accelY();
        aZ = mySensor.accelZ();
        float calibrated_ax = A_a[0][0] * (aX - b_a[0]) + A_a[0][1] * (aY - b_a[1]) + A_a[0][2] * (aZ - b_a[2]);

        // Integrate acceleration to get velocity
        velocityX += ((calibrated_ax * 9.8) + 0.1725) * dt;

        // Integrate velocity to get position
        positionX += velocityX * dt;

        Serial.println(positionX); // Print position in x direction
    }

    prevTime = currentTime;
}*/




//angle from gyro
/*
#include <MPU9250_asukiaaa.h>

MPU9250_asukiaaa mySensor;
float gZ;
float angleZ = 0;
unsigned long prevTime = 0;
float prevGZ = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  mySensor.beginGyro();
}

void loop() {
  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0; // Convert to seconds

  if (mySensor.gyroUpdate() == 0) {
    gZ = radians(mySensor.gyroZ()) - 0.012767;

    // Integration using trapezoidal rule
    angleZ += ((prevGZ + gZ) / 2.0) * dt;
    prevGZ = gZ;

    Serial.println(degrees(angleZ)); // Print the angle around z-axis
  }

  prevTime = currentTime; // Update previous time
}*/



/*
//angle from mag
#include <MPU9250_asukiaaa.h>
#include <Adafruit_BMP280.h>
#include <math.h>

// Define calibration parameters
float A_m[3][3] = {{0.649449, -0.004536, -0.008477},
                {-0.004536, 0.608403, -0.114700},
                {-0.008477, -0.114700, 0.664750}};
float b_m[3] = {21.926742, -26.039105, -146.304685};

MPU9250_asukiaaa mySensor;
float mX, mY, mZ;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();
  mySensor.setWire(&Wire);

  mySensor.beginMag();
}

void loop() {
  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();

    // Apply calibration parameters
    float calibrated_mx = A_m[0][0] * (mX - b_m[0]) + A_m[0][1] * (mY - b_m[1]) + A_m[0][2] * (mZ - b_m[2]);
    float calibrated_my = A_m[1][0] * (mX - b_m[0]) + A_m[1][1] * (mY - b_m[1]) + A_m[1][2] * (mZ - b_m[2]);
    
    // Calculate rotation angle along the z-axis
    float angle_z = atan2(calibrated_mx, calibrated_my) * 180 / PI;

    Serial.println(angle_z);
  }
}*/
