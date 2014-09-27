LaundryNotifier
===============
text here

---
## Requirements
#### Hardware
* Arduino Uno
* Adafruit CC3000 WiFi breakout board
* LM386 microphone amplifier
* electret microphone
* various capacitors and resistors (see wiring diagram in micRead folder)

#### Software

* [Arduino IDE](http://arduino.cc/en/main/software)

*Note: one of the following is required to setup a server to host the web interface*

* If using Mac OS X: [MAMP](http://www.mamp.info)
* Or for Windows, Linux, Mac OS X: [XAMPP](https://www.apachefriends.org/download.html)

---
## Setup
1. Download the project repo, IDE, and server software.
2. Place the *laundryInterface* folder in the *htdocs* directory of your server installation. When you are running your server, the web interface can be accessed from your browser by typing *localhost* in the address bar.
3. Edit the following files to reflect your phone number:
	* *htdocs/laundryInterface/php/scripts/textDryerDone.sh*
	* *htdocs/laundryInterface/php/scripts/textWasherDone.sh*
4. Place the *micRead* and *laundryNotify* folders in your Arduino *Sketchbook* folder.
5. Assemble the circuit according to *wiringDiagram.png* in the *micRead* folder.
6. Run the *micRead.ino* sketch to verify connections. Use this sketch to take acoustic readings of your washer/dryer machines in order to determine sound thresholds used in the main *laundryNotify.ino* sketch.
7. Edit *laundryNotify.ino* to reflect the following:
	* WLAN_SSID, WLAN_PASS, WLAN_SECURITY
	* Server IP address
	* Sound thresholds
8. Everything should be ready to go — upload the *laundryNotify.ino* sketch to the Uno + CC3000 and open the serial monitor to check if it connects to the network/server.
9. Go to the web interface and select which machine is running and then set the notifier:
![alt text](https://raw.githubusercontent.com/rmorrison23/LaundryNotifier/master/laundryInterface/interfaceScreenShot.png)     




