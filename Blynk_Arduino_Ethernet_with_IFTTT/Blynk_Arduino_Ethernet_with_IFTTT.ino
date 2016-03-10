/*
this is a porject for a device atteched with a led bar and DHT22。
It's made whih Blynk framwork.
It have a W5100 Ethernet shied atteched.
DHT22 data pin is digital_pin 2.
*/

/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 **************************************************************/
#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

#include <DHT.h>
#define DHTPIN 2 //DHT22 data pin
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

char auth[] = "b11ec4c53c1d4461abf44df332ade9ee";
char thingSpeakAddress[] = "api.thingspeak.com";
String APIKey = "LWT6NI7LO1464JXQ";

/*
 *Connect pin 1 (on the left) of the sensor to +5V
 *NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
 *to 3.3V instead of 5V!
 *Connect pin 2 of the sensor to whatever your DHTPIN is
 *Connect pin 4 (on the right) of the  sensor to GROUND
 *Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
 *
 *Initialize DHT sensor.
 *Note that older versions of this library took an optional third parameter to
 *tweak the timings for faster processors.  This parameter is no longer needed
 *as the current DHT reading algorithm adjusts itself to work on faster procs.
 */

DHT dht(DHTPIN, DHTTYPE);
EthernetClient client;
SimpleTimer timer,timer2;
float Temp, Humid;

void setup()
{
	timer.setInterval(3000L, putData_Blynk);
	timer2.setInterval(10000L, putData_thingspeak);
	Serial.begin(9600);
	dht.begin();
	Blynk.begin(auth);
	Serial.println("ready");
	Blynk.connect(2000);
}

void loop()
{
	timer.run();
	timer2.run();
	Blynk.run();
}

void putData_thingspeak() {
	String S_Temp = String(Temp);
	String S_Humid = String(Humid);
	updateThingSpeak("field1=" + S_Temp + "&field2=" + S_Humid);
}

void putData_Blynk() {
	// Reading temperature or humidity takes about 250 milliseconds!
	//read the data
	Temp = dht.readTemperature();
	Humid = dht.readHumidity();
	
	//output the data to Blynk
	Blynk.virtualWrite(V4, Humid);
	Blynk.virtualWrite(V5, Temp);
	Blynk.virtualWrite(V6, Humid);
	Blynk.virtualWrite(V7, Temp);
}


void updateThingSpeak(String tsData) {
	client.setTimeout(500);
	if (client.connect(thingSpeakAddress, 80)) {
		if (client.connected()) {
			Serial.println("Connecting to ThingSpeak...");
			Serial.println();
		}
	client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
	}
	client.stop();
}