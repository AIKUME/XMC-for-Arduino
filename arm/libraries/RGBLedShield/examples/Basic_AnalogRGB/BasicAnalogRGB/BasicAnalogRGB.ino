// You only need leds or an analogic RGB strip.

#include <RGBLedShield.h>

uint16_t Red_Intensity = 0;
uint16_t Green_Intensity = 0;
uint16_t Blue_Intensity = 0;
uint16_t Intensity = 4080;
uint16_t val = 0;



RGBLedShield rgb;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  rgb.start_shield();
}

void loop() {
  LEDoff();
    while (Red_Intensity < 4080) {
      increase_color (&Red_Intensity);
      rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
      delay(50);}
    while (Blue_Intensity < 4080) {
      increase_color (&Blue_Intensity);
      rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
      delay(50);}
    while (Red_Intensity > 0) {
      decrease_color (&Red_Intensity);
      rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
      delay(50);}
    while (Green_Intensity < 4080) {
      increase_color (&Green_Intensity);
      rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
      delay(50);}
    while (Blue_Intensity > 0) {
      decrease_color (&Blue_Intensity);
      rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
      delay(50);}
    while (Red_Intensity < 4080) {
      increase_color (&Red_Intensity);
      rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
      delay(50);}
    while (Blue_Intensity < 4080) {
      increase_color (&Blue_Intensity);
      rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
      delay(50);}
}



  void decrease_color(uint16_t *C) {
  Serial.println(*C);
  if ( *C == 4080)*C = 3840;
  else if (*C != 0)*C -= 320;
  Serial.println(*C);
}



void increase_color(uint16_t *C) {
  Serial.println(*C);
  if (*C == 3840) *C = 4080;
  else if (*C != 4080) *C += 320;
  Serial.println(*C);
}



void LEDoff(){
  Red_Intensity = 0;
  Green_Intensity = 0;
  Blue_Intensity = 0;
}

