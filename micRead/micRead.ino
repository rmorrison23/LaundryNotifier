// Convenience sketch to attain mic reads
	
int micPin = A4;
unsigned int sample;

const signed int sensitivity = -64; // dBV
const int gain = 26; //dB

const int numSamples = 10;
int samples[numSamples] = {0};
int n = 0;
int total = 0;
int avg = 0;

unsigned long previousMillis = 0;

void setup(){
    Serial.begin(9600);
}

void loop(){  
    micRead();
}

void micRead(void) {   

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
    Serial.print(convertToVolts(avg)); Serial.println(" Volts");
    Serial.print(convertToDecibels(convertToVolts(avg))); Serial.println(" dB");
}

int sampleAtFreq(int frequency) {
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

double convertToVolts(int input) {
    double volts = (input * 3.3) / 1024;  
    return volts;
}

double convertToDecibels(double input) {
    double reference = pow(10, sensitivity/20);
    double soundPressLevel = sensitivity + (20 * log10(input/reference)) + 94 - gain;
    return soundPressLevel;
}


