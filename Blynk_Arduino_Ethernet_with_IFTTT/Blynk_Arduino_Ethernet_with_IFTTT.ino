/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 *
 * This example shows how to use Arduino Ethernet shield (W5100)
 * to connect your project to Blynk.
 * Feel free to apply it to any other example. It's simple!
 *
 **************************************************************/
#define BLYNK_DEBUG
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>
#include "DHT.h"
#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "b11ec4c53c1d4461abf44df332ade9ee";

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

SimpleTimer timer,timer2;

//static const byte mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
//static const byte ip[] = { 192,168,0,3 };

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer ifttt(2000);

void setup()
{
  timer.setInterval(1000L, sendUptime);
  timer2.setInterval(2000L, sendUptime2);
  Serial.begin(9600);
  //  Ethernet.begin((byte*)mac,(byte*)ip);
  dht.begin();

  Blynk.begin(auth);
  // You can also specify server.
  // For more options, see BoardsAndShields/Arduino_Ethernet_Manual example
  //Blynk.begin(auth, "your_server.com", 8442);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8888);
  ifttt.begin();
  Serial.println("ready");
}

void sendUptime2(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  Blynk.virtualWrite(V4,dht.readHumidity());
  Blynk.virtualWrite(V5,dht.readTemperature());
  Blynk.virtualWrite(V6,dht.readHumidity());
  Blynk.virtualWrite(V7,dht.readTemperature());
}


//BLYNK_WRITE(V0) //Button Widget is writing to pin V1
//{
//  Blynk.terminal.print(myip);
//}

void sendUptime()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  EthernetClient ifttt_client = ifttt.available();
  if (ifttt_client) {
    Serial.println("new client");
    if (ifttt_client.connected()) {
      if (ifttt_client.available()) {
        if (ifttt_client.find("GET /")) {
          char light = ifttt_client.read();
          Serial.println(light);
          if (light == '1') {
            pinMode(5, OUTPUT);
            digitalWrite(5, HIGH);
          }
          if (light == '2') {
            pinMode(5, OUTPUT);
            digitalWrite(5, LOW);
          }
          if (light == '3') {
            pinMode(5, OUTPUT);
            for (int temp = 0; temp <= 5; temp++) {
              digitalWrite(5, LOW);
              delay(50);
              digitalWrite(5, HIGH);
              delay(50);
            }
          }
        }
      }
    }
  }
  ifttt_client.stop();
}


void loop()
{
  timer.run(); // Initiates SimpleTimer
  timer2.run();
  Blynk.run();
}

