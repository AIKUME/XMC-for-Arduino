  // You need leds or an analogic RGB strip and a Infineon 3D magnetic sensor 2Go.

  #include <RGBLedShield.h>
  #include <MagneticSensor3D.h>

  uint16_t Red_Intensity = 0;
  uint16_t Green_Intensity = 0;
  uint16_t Blue_Intensity = 0;
  uint16_t Intensity = 4080;
  float val = 0;

  int off = 0;
  int change;

  RGBLedShield rgb;
  
  void setup() {                

    Serial.begin(9600);
    Wire.begin();
    
    rgb.start_shield();

    while(!Serial);
    magnetic3dSensor.begin();
    magnetic3dSensor.setAccessMode(magnetic3dSensor.MASTERCONTROLLEDMODE);
    magnetic3dSensor.disableTemp();
  }


  
  void loop() {

    delay(magnetic3dSensor.getMeasurementDelay());
    magnetic3dSensor.updateData();

    if (magnetic3dSensor.getAmount() > 70) {
      change = (change+1) % 3;
      delay(1000);
    }
    
    val = magnetic3dSensor.getAzimuth();
    val = map(val, -3.15, 3.15, 0, 4080);
    Serial.println(val);

    switch(change){
      case 0:
        Red_Intensity = val;
      break;
      case 1:
        Green_Intensity = val;
      break;
      case 2:
        Blue_Intensity = val;
      break;
    }
    rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
  }  

