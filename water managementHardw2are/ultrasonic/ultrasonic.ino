#include <Ultrasonic.h>
Ultrasonic ultrasonic(2,3);// trig,echo
int distance;
void setup() {
  Serial.begin(9600);
    pinMode(2,OUTPUT);
  
  pinMode(3,INPUT);
}
void loop()
{  distance = ultrasonic.read();
Serial.println(distance);}
