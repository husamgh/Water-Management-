#define _DEBUG_
#define _DISABLE_TLS_
//#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ThingerWifi.h>
#include "EmonLib.h"
 #include <Ultrasonic.h>
#define USERNAME "husamgh"
#define DEVICE_ID "Node_MCU"
#define DEVICE_CREDENTIAL "lZG2@P%JsF%3"
WiFiClient client;

const char *ssid ="KOKO where are you";
const char *pass = "LOJ*^#_789mn";

ThingerWifi thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
EnergyMonitor emon1;    // Create an instance
Ultrasonic ultrasonic(5,4);//5 RX trig,4 TX echo, 
int  sum;
float  Irms, Vo;
float flowRate;
 

//---- for flow meter
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 12; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
// for the thermal resistor
const float resistance = 10000; 
float lecture_echo;
double distance,Dist,total;

void setup() {  
  Serial.begin(9600);
  thing.add_wifi(ssid, pass);
digitalWrite(1,HIGH); // to show that the node mcu is connected to the wireless
  emon1.current(A0,30);// Current: analog input pin, calibration. 

  thing["valve"] << [](pson& in) {if(in.is_empty()){
        in = (bool) digitalRead(0);
    }
    else{
        digitalWrite(0, in ? HIGH : LOW);
    }};
    
      thing["pump_activate"] << [](pson& in) {if(in.is_empty()){
        in = (bool) digitalRead(2);
    }
    else{
        digitalWrite(2, in ? HIGH : LOW);
    }};
      thing["SONIC"] >> [] (pson& out){

      Dist= ultrasonic.read();
//Dist=155-distance;
total=1000-(1000*(Dist/155));
 out=total;};
 
thing["SONIC_2"] >> [] (pson& out){
  distance = ultrasonic.read();
 out=distance;};
 
  thing["water pump"] >> [](pson & out) {
     Irms = emon1.calcIrms(2000);
    out = Irms;};


  thing["Flow meter"] >> [](pson & out) { 
    currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency  / 7.5); // (Pulse frequency) / 7.5Q = flowrate in L/min
      flow_frequency = 0; // Reset Counter
   }
   out=l_hour;
    };
  pinMode(5,OUTPUT);// Trigger pin RX
  pinMode(4,INPUT);// Echo pin TX
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(12,INPUT);
 digitalWrite(0,HIGH);
 digitalWrite(2,HIGH);
 
  //---flow meter
  pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);

 attachInterrupt(digitalPinToInterrupt(12),flow, RISING); // Setup Interrupt GPIO 14 
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
}

void loop() {
thing.handle();
}

ICACHE_RAM_ATTR void flow () // Interrupt function
{
   flow_frequency++;
}
