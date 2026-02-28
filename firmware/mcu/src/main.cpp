#include <Arduino.h>

const int NUM = 5;

float t = 0.0;
const float STEP = 0.1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < NUM; i++) {
    float phase = t+i*(2*PI/NUM);
    float s = sin(phase);
    int value = (int)((s+1.0)*50.0);
    
    Serial.print(value);

    if (i < NUM - 1) Serial.print("|");
  }

  Serial.println();

  t += STEP;
  if (t > 2*PI) t = -2*PI;

  delay(10);
}