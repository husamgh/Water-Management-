// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3


#include <LiquidCrystal.h>
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance
float Irms;

void setup()
{  
  Serial.begin(9600);
  
  emon1.current(13,30);  // Current: analog input pin, calibration.
}
void loop()
{ 
 
    Irms = emon1.calcIrms(2000);  // Calculate Irms only
//lcd.print(Irms*230.0);	       // Apparent power
 Serial.println("Irms is:");
 Serial.println(Irms);		       // Irms
  delay(500);
  
}
