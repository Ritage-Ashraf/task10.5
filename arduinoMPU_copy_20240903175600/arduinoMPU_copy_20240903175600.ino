#include <Wire.h>

const int MPU6050 = 0x68; //MPU6050 I2C address
float gyroZ;//Variable to store gyroscope data
float yaw;
unsigned long lastTime;//Stores the last time the loop ran.
float dt; //Time difference between readings

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // Initialize MPU6050
  Wire.beginTransmission(MPU6050);// Begins communication with the MPU6050.
  Wire.write(0x6B); // PWR_MGMT_1 register This register is used to manage the power settings of the MPU6050.
  Wire.write(0);    // Set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  // Set gyro configuration
  Wire.beginTransmission(MPU6050);
  Wire.write(0x1B); // Gyro configuration register  This register is used to configure the gyroscope settings, such as the full-scale range
  Wire.write(0x00); // Set full scale range to ±250 degrees/second
  Wire.endTransmission(true);
  
  lastTime = millis(); // Records the current time
}

void loop() {
  // Read gyroscope data
  Wire.beginTransmission(MPU6050);
  Wire.write(0x43); // Starting register for gyroscope readings
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 6, true); // Request 6 bytes of data
  
  int16_t gyroZRaw = Wire.read() << 8 | Wire.read();// Reads the raw gyroscope data for the Z-axis.
  
  // Convert raw data to degrees/second
  gyroZ = gyroZRaw / 131.0;
  
  // Calculate time difference
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;
  
  // Integrate gyroscope data to get yaw
  yaw += gyroZ * dt;
  
  // Print yaw
  Serial.print("Yaw: ");
  Serial.println(yaw);
  
  delay(100);
}
