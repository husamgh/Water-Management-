#define _DEBUG_
#include <SPI.h>
#include <Ethernet.h>
#include <ThingerEthernet.h>
 #include <Ultrasonic.h>

#define USERNAME "husamgh"
#define DEVICE_ID "Ard_uno"
#define DEVICE_CREDENTIAL "Wrq$@qJct&tE"

ThingerEthernet thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
// for the ultrasonic sensor
Ultrasonic ultrasonic(2,3);//2 trig,3 echo,
double distance;
// down for the thermal resistor
int ThermistorPin =0;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


void setup() {
Serial.begin(9600);

 thing["Ultra_Sonic"] >> [] (pson& out){
  distance = ultrasonic.read();
 out=distance;};
 
thing["Temperature"] >> [] (pson& out){
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  out=Tc;
};
thing["Box_Cover"] >> [](pson & out) {
 if(digitalRead(4)==LOW)
  digitalWrite(5,HIGH);
if(digitalRead(4)==HIGH)
digitalWrite(5,LOW);
    out =digitalRead(4);};
pinMode(6,OUTPUT);
   pinMode(4,INPUT);
  pinMode(5,OUTPUT);
  pinMode(3,INPUT);
   pinMode(0,INPUT);
  pinMode(2,OUTPUT);
digitalWrite(6,HIGH);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
}
void loop() {
  thing.handle();
}
