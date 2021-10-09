#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>
#include <L3G.h> //Polulo
//9-Axis L3GD20/LSM303D 9DOF IMU Breakout Pressure Digital Gyroscope Sensor Module - LSM303DLHC

/* Assign a unique ID to the sensors */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
L3G gyro;

void displaySensorDetails(void)
{
  sensor_t sensor;

  accel.getSensor(&sensor);
  Serial.println(F("----------- ACCELEROMETER ----------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" m/s^2"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  mag.getSensor(&sensor);
  Serial.println(F("----------- MAGNETOMETER -----------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" uT"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" uT"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" uT"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  gyro.read();
  Serial.print("G ");
  Serial.print("X: ");
  Serial.print((int)gyro.g.x);
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y);
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z);
  Serial.println();

  delay(500);
}

//Time keeping variables
unsigned long curMillis;
unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 200;

//Led flash Variables
unsigned long prevLedChase = 0;
unsigned long ChaseInterval = 500;
int newFlashInterval = 500;

int RedLed = 3;
int YellowLed = 5;
int GreenLed = 6;
int BlueLed = 9;
int range = 0;

void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Adafruit 9DOF Tester")); Serial.println("");
  pinMode(RedLed, OUTPUT);
  pinMode(YellowLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(BlueLed, OUTPUT);

  /* Initialise the sensors */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while (1);
  }

  if (!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  gyro.enableDefault();

  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop(void)
{
  curMillis = millis();

  /* Get a new sensor event */
  sensors_event_t event;

  /* Display the results (acceleration is measured in m/s^2) */
  accel.getEvent(&event);
  Serial.print(F("ACCEL "));
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m/s^2 ");

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  mag.getEvent(&event);
  Serial.print(F("MAG   "));
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  "); Serial.println("uT");

  /* the raw
    reading of 345 corresponds to 345 * 8.75 = 3020 mdps = 3.02 dps.) */
  gyro.read();
  Serial.print(F("GYRO   "));
  Serial.print("X: "); Serial.print((int)gyro.g.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print((int)gyro.g.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print((int)gyro.g.z); Serial.print("  "); Serial.println(F("dps"));

  //delay(1000);

  if (curMillis - prevLedChase >= ChaseInterval) {
    prevLedChase = millis(); S

    switch (range) {
      case 0:
        digitalWrite(RedLed, HIGH);
        break;
      case 1:    // your hand is close to the sensor
        // turn the pin off:
        digitalWrite(RedLed, LOW);
        break;
      case 2:    // your hand is a few inches from the sensor
        // turn the pin off:
        digitalWrite(YellowLed, HIGH);
        break;
      case 3:    // your hand is nowhere near the sensor
        digitalWrite(YellowLed, LOW);
        break;
      case 4:
        digitalWrite(GreenLed, HIGH);
        break;
      case 5:    // your hand is close to the sensor
        // turn the pin off:
        digitalWrite(GreenLed, LOW);
        break;
      case 6:    // your hand is a few inches from the sensor
        // turn the pin off:
        digitalWrite(BlueLed, HIGH);
        break;
      case 7:    // your hand is nowhere near the sensor
        digitalWrite(BlueLed, LOW);
        break;
    }
    range++;
    if (range == 8) {
      range = 0;
    }

  }
}
