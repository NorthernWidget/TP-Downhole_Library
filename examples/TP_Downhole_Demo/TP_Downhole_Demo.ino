#include "TP_Downhole.h"
#include <MCP3421.h>
#include <MS5803.h>

TP_Downhole Sensor;

void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to the Machine...\n\n");
  Sensor.begin(TP2v2);
}

void loop() {
  float Pressure = Sensor.getPressure();
  float Temp0 = Sensor.getTemperature(0);
  float Temp1 = Sensor.getTemperature(1);
  Serial.print("Pressure = "); 
  Serial.print(Pressure);
  Serial.println(" mBar");
  Serial.print("Temp0 = ");
  Serial.print(Temp0);
  Serial.print("\tTemp1 = ");
  Serial.println(Temp1);
  Serial.print("\n\n");

  delay(1000);
}
