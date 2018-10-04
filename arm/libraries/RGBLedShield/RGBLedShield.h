#ifndef RGBLedShield_h
#define RGBLedShield_h


#include "Arduino.h"
#include <Wire.h>

/****************************************************************
* MACROS
***************************************************************/

  #define ADDRESS                    0x15EUL

  #define INTENSITY_RED              0x11U
  #define INTENSITY_GREEN            0x12U
  #define INTENSITY_BLUE             0x13U
  #define INTENSITY_RGB              0x14U
  #define CURRENT_RED                0x21U
  #define CURRENT_GREEN              0x22U
  #define CURRENT_BLUE               0x23U
  #define CURRENT_RGB                0x24U
  #define DMXOFF                     0x30U
  #define DMXON                      0x31U
  #define DMXSLOT                    0x32U
  #define DMX8BIT                    0x33U
  #define DMX16BIT                   0x34U
  #define OFFTIME_RED                0x41U
  #define OFFTIME_GREEN              0x42U
  #define OFFTIME_BLUE               0x43U
  #define WALKTIME                   0x50U
  #define DIMMINGLEVEL               0x60U
  #define FADERATE                   0x61U
  #define READ_INTENSITY_RED         0x81U
  #define READ_INTENSITY_GREEN       0x82U
  #define READ_INTENSITY_BLUE        0x83U
  #define READ_CURRENT_RED           0x84U
  #define READ_CURRENT_GREEN         0x85U
  #define READ_CURRENT_BLUE          0x86U
  #define READ_OFFTIME_RED           0x87U
  #define READ_OFFTIME_GREEN         0x88U
  #define READ_OFFTIME_BLUE          0x89U
  #define READ_WALKTIME              0x8AU
  #define READ_DIMMINGLEVEL          0x8BU
  #define READ_FADERATE              0x8CU

  #define BCCUMODID                  0x50030008U
  #define CHIPID                     0x40010004U
  #define REDINTS                    0x500300A0U
  #define REDINT                     0x500300A4U
  #define BLUEINTS                   0x50030078U
  #define STARTWALK                  0x50030018U

class RGBLedShield{

  public:
    RGBLedShield();
    void I2CWRITE2BYTES (int Address, int Command, unsigned int Data);
    void write_intensity( uint16_t Red_Intensity, uint16_t Green_Intensity, uint16_t Blue_Intensity, uint16_t Intensity);
    void I2CWRITE6BYTES (unsigned int Address, unsigned int Command, unsigned int DataOne, unsigned int DataTwo, unsigned int DataThree);
    unsigned int I2CREAD (unsigned int Address, unsigned int Command);
    void start_shield();

  private:
    unsigned int c[2] = {0};
    unsigned int d[4] = {0};
    unsigned int on = 0;
    unsigned int message = 0;
    unsigned long redcurr = 0;
    unsigned long greencurr = 0;
    unsigned long bluecurr = 0;
    unsigned long redoff = 0;
    unsigned long greenoff = 0;
    unsigned long blueoff = 0;
    unsigned long redint = 0x00;
    unsigned long greenint = 0x00;
    unsigned long blueint = 0x00;
    unsigned long fadetime = 0x00;
    unsigned long walk = 0x00;
    unsigned long brightness = 1;
};

#endif
