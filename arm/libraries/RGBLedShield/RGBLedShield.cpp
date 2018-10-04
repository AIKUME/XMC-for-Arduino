
#include "RGBLedShield.h"

  /****************************************************************
  * FUNCTIONS
  ***************************************************************/

  /*
   Parameters (IN): int Address - Address of RGB LED Shield, Default 0x15E
                    int Command - Defined I2C Commands i.e. INTENSITY_RED, INTENSITY_GREEN, INTENSITY_BLUE
                    unsigned int Data - 16bit data to be written to slave
   Parameters (OUT): None
   Return Value: None
   Description: This function will write 2 bytes of word to the I2C bus line
   */

   RGBLedShield::RGBLedShield(){}

   void RGBLedShield::I2CWRITE2BYTES (int Address, int Command, unsigned int Data)
   {
     unsigned int upperByte, lowerByte; // Separate 4 byte data into 2 byte values
     lowerByte = Data;
     upperByte = Data >> 8;

     unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF); // Putting address into correct format
     unsigned int upperSLAD = Address >> 8;
     upperSLAD |= 0x79; // First 5 bits 11110 and last bit '1' for a write

     Wire.beginTransmission(byte(upperSLAD)); // Start I2C transmission
     Wire.write(byte(lowerSLAD)); // address lower 8 bits of i2c address
     Wire.write(byte(Command)); // write command
     Wire.write(byte(upperByte)); // write data
     Wire.write(byte(lowerByte));
     Wire.endTransmission(true);

   }



   void RGBLedShield::write_intensity( uint16_t Red_Intensity, uint16_t Green_Intensity, uint16_t Blue_Intensity, uint16_t Intensity) {
     I2CWRITE2BYTES(ADDRESS, INTENSITY_RED, Red_Intensity);
     I2CWRITE2BYTES(ADDRESS, INTENSITY_GREEN, Green_Intensity);
     I2CWRITE2BYTES(ADDRESS, INTENSITY_BLUE, Blue_Intensity);
     I2CWRITE2BYTES (ADDRESS, DIMMINGLEVEL, Intensity);
   }



    /*
   Parameters (IN): int Address - Address of RGB LED Shield, Default 0x15E
                    int Command - Defined I2C Commands i.e. INTENSITY_RGB, CURRENT_RGB
                    unsigned int DataOne, unsigned int DataTwo, unsigned int DataThree - Three 16bit data to be written to slave
   Parameters (OUT): None
   Return Value: None
   Description: This function will write 6 bytes of word to the I2C bus line
   */

   void RGBLedShield::I2CWRITE6BYTES (unsigned int Address, unsigned int Command, unsigned int DataOne, unsigned int DataTwo, unsigned int DataThree) // DataOne: Red, DataTwo: Green, DataThree: Blue
   {
     unsigned int upperByte, lowerByte; // Split each Data parameter into upper and lower 8 bytes because I2C format sends 8 bytes of data each time
     lowerByte = DataOne;
     upperByte = DataOne >> 8;

     unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF);
     unsigned int upperSLAD = Address >> 8;
     upperSLAD |= 0x79; // First 5 bits 11110 and last bit '1' for a write

     Wire.beginTransmission(byte(upperSLAD)); // Red
     Wire.write(byte(lowerSLAD));
     Wire.write(byte(Command));
     Wire.write(byte(upperByte));
     Wire.write(byte(lowerByte));
     lowerByte = DataTwo;
     upperByte = DataTwo >> 8;
     Wire.write(byte(upperByte));
     Wire.write(byte(lowerByte));
     lowerByte = DataThree;
     upperByte = DataThree >> 8;
     Wire.write(byte(upperByte));
     Wire.write(byte(lowerByte));
     Wire.endTransmission(true);

   }


    /*
   Parameters (IN): int Address - Address of RGB LED Shield, Default 0x15E
                    int Command - Defined read I2C Commands i.e. READ_INTENSITY_RED, READ_INTENSITY_GREEN, READ_INTENSITY_BLUE
   Parameters (OUT): None
   Return Value: Requested data from Shield will be sent back
   Description: This function will request 2 bytes of word from the shield
   */

   unsigned int RGBLedShield::I2CREAD (unsigned int Address, unsigned int Command) // Returns data sent by slave
   {
     int i = 0;
     unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF);
     unsigned int upperSLAD = Address >> 8;
     upperSLAD |= 0x79;

     Wire.beginTransmission(byte(upperSLAD)); // Red
     Wire.write(byte(lowerSLAD));
     Wire.write(byte(Command));
     Wire.endTransmission(false); // false for Repeated Start

     Wire.beginTransmission(byte(upperSLAD));
     Wire.write(byte(lowerSLAD));
     Wire.requestFrom(upperSLAD, 2, true);
     unsigned int data = 0;

     while(Wire.available())    // slave may send less than requested. Print out received data byte
     {
       message = 1;
       c[i] = Wire.read(); // receive a byte as character
       i++;

     }
     Wire.endTransmission(true);

     data = c[1]; // write data to serial monitor. c[1] is higher byte
     data = (data << 8) | c[0];  // shift left and combine with lower byte
     return data;
   }



   void RGBLedShield::start_shield() {
     while (on != 1) // Wait for shield to respond
      {
        I2CWRITE2BYTES (ADDRESS, FADERATE, 0x0000);         // Immediate fade
        I2CWRITE2BYTES (ADDRESS, DIMMINGLEVEL, 0x0000);     // 0% brightness level
        on = I2CREAD(ADDRESS, READ_DIMMINGLEVEL);           // Request for brightness level

        if (message == 1 && on == 0)                         // If message received and brightness level = 9%
        {
          message = 0;
          on = 1;                                             // break out of loop
        }
      }

      while (redcurr != 0x15 || greencurr != 0x15 || bluecurr != 0x15 || redoff != 0x38 || greenoff != 0x39 || blueoff != 0x38 || brightness != 0)  //Set the security parameters
      {
          I2CWRITE6BYTES (ADDRESS, INTENSITY_RGB, 0x0000, 0x000, 0x0000);         // Off Light

      // Ensure that parameters are set up correctly. Read back and check. If wrong, write and read again.
          redcurr = I2CREAD (ADDRESS, READ_CURRENT_RED);                          // Read the red current intensity
          greencurr = I2CREAD (ADDRESS, READ_CURRENT_GREEN);                      // Read the green current intensity
          bluecurr = I2CREAD (ADDRESS, READ_CURRENT_BLUE);                        // Read the blue current intensity
          redoff = I2CREAD (ADDRESS, READ_OFFTIME_RED);                           // Read the off-time of the red channel
          greenoff = I2CREAD (ADDRESS, READ_OFFTIME_GREEN);                       // Read the off-time of the green channel
          blueoff = I2CREAD (ADDRESS, READ_OFFTIME_BLUE);                         // Read the off-time of the blue channel
          brightness = I2CREAD (ADDRESS, READ_DIMMINGLEVEL);                      // Read the dimming level

          I2CWRITE2BYTES (ADDRESS, OFFTIME_RED, 0x38);                            // Set off-time of red channel to 0x38
          I2CWRITE2BYTES (ADDRESS, OFFTIME_GREEN, 0x39);                          // Set off-time of green channel to 0x39
          I2CWRITE2BYTES (ADDRESS, OFFTIME_BLUE, 0x38);                           // Set off-time of blue channel to 0x38
          I2CWRITE2BYTES (ADDRESS, CURRENT_RED, 0x15);                            // Set current intensity of red channel to 0x15
          I2CWRITE2BYTES (ADDRESS, CURRENT_GREEN, 0x15);                          // Set current intensity of green channel to 0x15
          I2CWRITE2BYTES (ADDRESS, CURRENT_BLUE, 0x15);                           // Set current intensity of blue channel to 0x15
          I2CWRITE2BYTES (ADDRESS, DIMMINGLEVEL, 0x0000);
      }

    I2CWRITE2BYTES (ADDRESS, WALKTIME, 0xF);
    I2CWRITE2BYTES (ADDRESS, FADERATE, 20);               /* Fade Rate to 1.4s */
    I2CWRITE2BYTES (ADDRESS, DIMMINGLEVEL, 2048);
   }
