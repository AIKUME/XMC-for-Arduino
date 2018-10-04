 // You need leds or an analogic RGB strip and a gesture sensor APDS-9960.
  
  #include <RGBLedShield.h>
  #include <SparkFun_APDS9960.h>  

  uint16_t Red_Intensity = 0;
  uint16_t Green_Intensity = 0;
  uint16_t Blue_Intensity = 0;
  uint16_t Intensity = 4080;
  uint16_t val = 0;

  int isr_flag = 0;
  int off = 0;
  int crazy = 0;
  int change;

  RGBLedShield rgb;
  SparkFun_APDS9960 apds = SparkFun_APDS9960();
  
  void setup() {                

    Serial.begin(9600);
    Wire.begin();
        
    rgb.start_shield();

       attachInterrupt(0, interruptRoutine, FALLING);
    if ( apds.init() ) {
      Serial.println(F("APDS-9960 initialization complete"));
    } else {
      Serial.println(F("Something went wrong during APDS-9960 init!"));
    }
    if ( apds.enableGestureSensor(true) ) {
      Serial.println(F("Gesture sensor is now running"));
    } else {
      Serial.println(F("Something went wrong during gesture sensor init!"));
    }
  }


  
  void loop() {
   if( isr_flag == 1 ) {
      isr_flag = 0;
      detachInterrupt(0);
      attachInterrupt(0, interruptRoutine, FALLING);
      isr_flag = 0;
      handleGesture();
    }
  }  



  void handleGesture() {
      if ( apds.isGestureAvailable() ) {
      switch ( apds.readGesture() ) {
        case DIR_UP:
          if (off == 1) off++;
          else off = 0;
          crazy = 0;
          Serial.println("UP");
          increase_color(&Red_Intensity);
          break;
        case DIR_DOWN:
          if (off == 0) off++;
          else if (off == 2){
            off = 0;
            LEDoff();
          }
          crazy = 0;
          Serial.println("DOWN");
          decrease_color(&Red_Intensity);
          break;
        case DIR_LEFT:
        Serial.print("CRAZY. ");
        Serial.println(crazy);
          if (crazy == 0) crazy++;
          else if (crazy == 2){
            crazy = 0;
            crazyFUN();
          }
          off = 0;
          Serial.println("LEFT");
          decrease_color(&Blue_Intensity);
          break;
        case DIR_RIGHT:
        Serial.print("CRAZY. ");
        Serial.println(crazy);
          if (crazy == 1) crazy++;
          else crazy = 0;
          off = 0;
          Serial.println("RIGHT");
          increase_color(&Blue_Intensity);
          break;
        case DIR_NEAR:
          off = 0;
          crazy = 0;
          Serial.println("NEAR");
          decrease_color(&Green_Intensity);
          break;
        case DIR_FAR:
          off = 0;
          crazy = 0;
          Serial.println("FAR");
          increase_color(&Green_Intensity);
          break;
        default:
          Serial.println("NONE");
          off = 0;
          crazy = 0;
      }
    }
    rgb.write_intensity(Red_Intensity, Green_Intensity, Blue_Intensity, Intensity);
  }

  

  void interruptRoutine() {
    isr_flag = 1;
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
  


  void crazyFUN(){
    while ( isr_flag == 0 ) {
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
  }

