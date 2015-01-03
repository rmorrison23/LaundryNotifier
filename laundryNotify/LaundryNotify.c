/***********************************************************************************
	Arduino sketch to interface an electret microphone and MAMP server using
	the Adafruit CC3000 breakout board.

	Author: Ryan Morrison 

	The sketch queries the server to assess if the notifier is set. If it is set,
	the server is queried for which machine (washer, dryer, or both) is running.
	Notifier/machine status is updated by the user via a web page. 

	Mic reads and sound threshold comparisons are used to determine when a machine
	cycle is done; a text message is then sent to the user.

	A tremendous amount of help for this project was attained from the following:

		-- Marco Schwartz http://www.openhomeautomation.net
		-- http://bitknitting.wordpress.com	 
************************************************************************************/
		
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include "utility/debug.h"
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// WiFi network parameters (change as appropriate)
#define WLAN_SSID       "HAL 9000"
#define WLAN_PASS       "fat323/un5p0k3n"
#define WLAN_SECURITY   WLAN_SEC_WPA2 

// Max time to wait for address from DHCP
const unsigned long dhcpTimeout = 60L * 1000L;
uint32_t t;

// CC3000 and client instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2);
Adafruit_CC3000_Client client;

// Server IP and port (change as appropriate)
uint32_t ip = cc3000.IP2U32(10,0,1,6);
int port = 80;

// Server GET requests (Xively is not needed for this sketch to work)
const char machine_state[] = "GET /laundryNotify/machineStatus.txt HTTP/1.0\r\nConnection: close\r\n\r\n";
const char notifier_state[] = "GET /laundryNotify/notifierStatus.txt HTTP/1.0\r\nConnection: close\r\n\r\n";
const char notifier_off[] = "GET /laundryNotify/notifierOff.php HTTP/1.0\r\nConnection: close\r\n\r\n";
const char washer_done[] = "GET /laundryNotify/washerDone.php HTTP/1.0\r\nConnection: close\r\n\r\n";
const char dryer_done[] = "GET /laundryNotify/dryerDone.php HTTP/1.0\r\nConnection: close\r\n\r\n";
const char reset_machines[] = "GET /laundryNotify/resetXively.php HTTP/1.0\r\nConnection: close\r\n\r\n";

// Holds machine and notifier status from server
String readState;

// Microphone related globals (adjust thresholds according to your acoustics)
int micPin = A4;
const int numSamples = 10;
const unsigned int silenceThresh = 550;
const unsigned int noiseThresh = 560;
const unsigned int buzzerThresh = 1000;

unsigned long previousMillis = 0;
unsigned int maxCycleTime = 3600000;

void setup(void) {
  
  	Serial.begin(57600);
  	//Serial.begin(9600);

  	Serial.print(F("Initializing..."));
  	if (!cc3000.begin()) {
    	Serial.println(F("failed. Check your wiring?"));
    	return;
  	}

  	Serial.print(F("OK.\r\nConnecting to network..."));
  	cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  	Serial.println(F("connected!"));

  	Serial.print(F("Requesting address from DHCP server..."));
  	for (t=millis(); !cc3000.checkDHCP() && ((millis() - t) < dhcpTimeout); delay(500)) {
    	Serial.println("....waiting");
  	}

  	if (cc3000.checkDHCP()) {
    	Serial.println(F("OK"));
  	} 
  	else {
    	Serial.println(F("failed"));
    	return;
  	}
}

void loop(void) {
  
	Serial.println(F("Checking notifier status..."));
    
    // keep looping until server reports notifier is set
    if (send_request(notifier_state) == "Notifier Set\n") {
    	Serial.println(F("Notifier is set!"));
    
    	if (send_request(machine_state) == "Washer ON\n") {
        	handleWasher();
       	}
    	delay(1000);

       if (send_request(machine_state) == "Dryer ON\n") {
        	handleDryer();
       	}
       	delay(1000);
       
       if (send_request(machine_state) == "Both ON\n") {
        	int firstOneDone = handleBoth();
        
        	switch (firstOneDone) {
          		case 1:
            	handleDryer();
            	break;
          	case 2:
            	handleWasher();
            	break;
          	default:
            	break;
        	}
       	}
       	delay(1000);
    }
    delay(5000);
}

/********************************************************************************
 * Sends request to server.
 ********************************************************************************/
String send_request (String request) {
  
	Serial.println(F("Starting connection to server..."));
    client = cc3000.connectTCP(ip, port);
    readState = "";
    
    // Send request
    if (client.connected()) {
    	client.println(request);      
      	client.println(F(""));
      	Serial.println(F("Connected & Data sent"));
    } 
    else {
      	Serial.println(F("Connection failed"));    
    }

    while (client.connected()) {
    	while (client.available()) {
        
        	// Read answer
        	char c = client.read();
        	Serial.print(c);
        	
        	// look for flag indicating beginning of status data
       		if (c == '*') {
          		while (client.connected()) {
            		while (client.available()) {
             			while (c!= '\n') { 
               				c = client.read();
               				readState += c;
             			}
            		}
          		}
        	}
      	}
    }

    // Close connection to server
    Serial.println(F("Closing connection"));
    Serial.println(F("")); 
    client.close();
    Serial.println(F("Stopping the connection..."));  
    delay(1000);

    return readState;
}

/********************************************************************************
 * Monitors washer run time and sound levels; sends text alert when done.
 ********************************************************************************/
void handleWasher(void) {
  	Serial.println(F("Washer is running!")); 
  	unsigned int positiveCount = 0;
  	unsigned long currentMillis = millis();
  
  	// check if elapsed time exceeds max washer run-time
  	if (currentMillis - previousMillis > maxCycleTime) {
    	previousMillis = currentMillis;
    	Serial.println(F("Washer cycle complete, sending text alert!"));
    	Serial.println(send_request(washer_done)); 
    	Serial.println(send_request(reset_machines));
    	Serial.println(send_request(notifier_off));
  	}
  	delay(1000);
    
    // check for spin cycle or buzzer 
  	if (micRead() > noiseThresh) {
    	Serial.println(F("End of cycle detected, checking for errors..."));
  
    	// sample mic to check for erroneous ambient sound
    	for (int i = 0; i < 20; i++) {
      		if (micRead() > noiseThresh) {
        		positiveCount++;  
      		}
    	}
  	}
	
	// send alert once spin cycle completes/buzzer sounds   
  	if (positiveCount >= 10) {
    	while (micRead() > silenceThresh) {
    	}
    	Serial.println(F("Washer cycle complete, sending text alert!"));
    	Serial.println(send_request(washer_done)); 
    	Serial.println(send_request(reset_machines));
    	Serial.println(send_request(notifier_off));
  	}
}

/********************************************************************************
 * Monitors dryer sound levels; sends text alert when done.
 ********************************************************************************/
void handleDryer(void) {
  	Serial.println(F("Dryer is running!"));
  	unsigned int positiveCount = 0;
  	
  	// check for silence 
  	while (micRead() > noiseThresh) {
    	delay(1000);
  	}
  
  	// sample mic to check for erroneous ambient sound
  	if (micRead() <= noiseThresh) {
  		Serial.println(F("End of cycle detected, checking for errors..."));
     	for (int i = 0; i < 20; i++) {
       		if (micRead() < noiseThresh) {
         		positiveCount++;  
       		}
     	}
  	}
  	
  	// send alert once dryer is done
  	if (positiveCount >= 10){
    	Serial.println(F("Dryer cycle complete, sending text alert!"));
    	Serial.println(send_request(dryer_done)); 
    	Serial.println(send_request(reset_machines));
    	Serial.println(send_request(notifier_off));
 	}
}

/********************************************************************************
 * Monitors both machines and hands off to dryer/wahser routines when one is done
 ********************************************************************************/
int handleBoth(void) {
  	Serial.println(F("Both machines are running!"));
  	unsigned int positiveCount = 0;
  	
  	// loop while dryer is running
  	while (micRead() > noiseThresh) {
      	unsigned long currentMillis = millis();

      	// covers the case where dryer runs longer than washer
      	if (currentMillis - previousMillis > maxCycleTime) {
        	previousMillis = currentMillis;
        	Serial.println(F("Washer cycle complete, sending text alert!"));
        	Serial.println(send_request(washer_done));
        	return 1;
      	}
  	}
  	
  	// checks for dryer done (washer runs longer than dryer)
  	for (int i = 0; i < 20; i++) {
    	if (micRead() <= noiseThresh) {
      		positiveCount++;  
    	}
  	}
  
  	if (positiveCount >= 10) {
    	Serial.println(send_request(dryer_done));
    	return 2;
  	}
}

/********************************************************************************
 * Returns the average max read of 200 samples
 ********************************************************************************/
int micRead(void) { 

	int samples[numSamples] = {0};
	int n = 0;
	int total = 0;
	int avg = 0;
   
  	for (int i = 0; i < 200; i++) {
    	total -= samples[n];
    	samples[n] = sampleAtFreq(20);
    	total += samples[n];
    	n += 1;
    	if (n >= numSamples) {
      		n = 0;
    	}
    	avg = total / numSamples;
  	}
  	Serial.println(avg);
  	//Serial.print(convertToVolts(avg)); Serial.println(" Volts");
  	//Serial.print(convertToDecibels(convertToVolts(avg))); Serial.println(" dB");
  	return avg;
}

/********************************************************************************
 * Returns the analog signal max at a given frequency
 ********************************************************************************/
int sampleAtFreq(int frequency){

	unsigned int sample;
  	int sampleTime = (1000.0/frequency + 0.5);
  	unsigned long startMillis = millis(); 
  	unsigned int peakToPeak = 0;  
  	unsigned int signalMax = 0;
  	unsigned int signalMin = 1024;
   
  	while (millis() - startMillis < sampleTime) {
    	sample = analogRead(micPin);
    	if (sample < 1024) {
      		if (sample > signalMax) {
        		signalMax = sample;  
      		}
      		else if (sample < signalMin) {
        		signalMin = sample;  
      		}
    	}
  	}
  	peakToPeak = signalMax - signalMin; 
  	return signalMax;
}

/********************************************************************************
 * Convenience function to convert micRead to Volts
 ********************************************************************************/
/*double convertToVolts(int input) {
  	double volts = (input * 3.3) / 1024;  
  	return volts;
}*/

/********************************************************************************
 * Convenience function to convert micRead to Decibels
 ********************************************************************************/
/*double convertToDecibels(double input){
	signed int sensitivity = -64; // dBV
	int gain = 26; // dB
  	double reference = pow(10, sensitivity/20);
  	double soundPressLevel = sensitivity + (20 * log10(input/reference)) + 94 - gain;
  	return soundPressLevel;
}*/