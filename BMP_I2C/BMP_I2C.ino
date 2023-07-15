#include <Adafruit_BMP3XX.h>
#include <bmp3.h>
#include <bmp3_defs.h>
#include <Wire.h>

# define SDA 0
# define SCL 1

const int BMP388_ADDRESS = 0x77;  // BMP388 I2C address

uint32_t readData(uint8_t reg, uint8_t len) {
  Wire.beginTransmission(BMP388_ADDRESS);
  Wire.write(reg); // Send the register to read data from
  Wire.endTransmission();
  
  Wire.requestFrom(BMP388_ADDRESS, len); // request to read 'len' bytes from the sensor
  
  uint32_t data = 0; // Variable to hold the data
  data = Wire.read(); //read LSB (byte 0)
  return data;
}

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(BMP388_ADDRESS);
  Wire.write(reg); // Sends the register to write to
  Wire.write(value); // Sends the value to write
  Wire.endTransmission();
}

float readTemperature() {
  uint32_t rawData = readData(0x07, 3); // Collect raw data
  int32_t temperature = (rawData << 16) | (rawData << 8);
  float temp_converted = temperature; // Convert data to temperature in degree celcius

  return temp_converted;
}

float readPressure() {
  uint32_t rawData = readData(0x04, 3); // Collect raw data 
  int32_t pressure = (rawData >> 4);
  
  float pressure_compensation = ((float)pressure / 100.0); // Apply compensation factor to raw data 

  float pressure_converted = 0.00001525878*pressure; // Convert data to temperature in degree celcius

  return pressure_converted;
}


void setup() {
  Wire.begin(SDA, SCL);
  Serial.begin(115200);
  delay(2000);
  
  // Initialize the BMP388 sensor
  writeRegister(0x7E, 0xB6); // Soft reset the sensor
  delay(100);
  writeRegister(0x1C, 0x01); // Set the sensor mode to normal
}

void loop() {
  // Read temperature and pressure data from BMP388 sensor
  float temperature = readTemperature();
  float pressure = readPressure();
   
  // Print the data to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" Pa");
  
  delay(1000);
}
