/*----------------------------------------------------------------------------------------------------
  BOARD 2 PROGRAM (DATA LOGGING)

 The Board 2 program uses multiple libraries including "SD.h", "HX711_ADC.h", "SPI.h", and
 "Wire.h". The SD card CS pin is connected to Arduino D10 pin, and for HX711, DOUT is
 connected to D5, and SCK pin is connected to D6. The program has a baud rate of 9600, and the
 calibration value should be written in the "calibrationValue" section as mentioned in the
 program. The program starts up the load cell and starts to read the load cell value in grams. Finally, it
 writes the data to the SD card while simultaneously reading the data.


 ALGORITHM:
 1. Include the SD, HX711_ADC, SPI, and Wire libraries.
 2. Initialize a File object for the data file on the SD card.
 3. Initialize an HX711_ADC object with the appropriate pins and calibration value..
 4. Enter a while loop that runs indefinitely.
 5. Read the load cell value in grams using the HX711_ADC object.
 6. Open the data file on the SD card.
 7. Write the current time and load cell value to the data file.
 8. Close the data file.
 9. Wait for a specified delay before reading the load cell value again.
 10. End the program.

 Refer GitHub repository for complete details: https://github.com/SPARK-TECH-INDUSTRIES/SPECTRA

  Designed by SPARK TECH INDUSTRIES:
  https://www.thesparktech.xyz/

  ----------------------------------------------------------------------------------------------------
*/

#include <SD.h>
#include <HX711_ADC.h>
#include <SPI.h>
#include <Wire.h>

#define SD_CS (10)

const int HX711_dout = 5; 
const int HX711_sck = 6;

HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;
File dataFile;

void setup()
{
  Serial.begin(9600);
  SD.begin(SD_CS);
  LoadCell.begin();

  float calibrationValue;
  calibrationValue = 96.32; // Define the calibration value obtained from calibration program

  unsigned long stabilizingtime = 2000;
  boolean _tare = true;
  LoadCell.start(stabilizingtime, _tare);  

  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("Startup is complete");
  }
  
}

void loop()
{
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0;

   if (LoadCell.update()) newDataReady = true;

   if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
  
  dataFile = SD.open("dataFile.txt",FILE_WRITE);
  dataFile.println(String(LoadCell.getData()));
  dataFile.close();

}
