#include <SPI.h>
#include <SD.h>
#include "DS3231.h"
#include <Wire.h>

int soilval = 0; //value for storing moisture value
int lightval = 0; //value for storing light level
int tempval = 0; //value for storing temp level
int soilPin = A0;//Declare a variable for the soil moisture sensor
int lightPin = A1; //Declare a variable for the light sensor
int tempPin = A2; //Declare a variable for the temp sensor

// LED pins
int blueLED = 5;
int yellowLED = 6;
int greenLED = 7;

//flags for LEDs
int green = 1;
int blue = 0;
int yellow = 0;

//values
float tempAVG = -1;
float lightAVG = -1;

//default values for health check
//All values are defined as a % from 0 to 99
int waterlow = 30;
int templow = 30;
int temphigh = 80;
int lightAVGlow = 20;
int lightAVGhigh = 80;
//default reading Freq in readings per hour
int readingFreq = 1;
int desiredSize = 24 * readingFreq;

File logfile, configfile;
char receivedChars[32];
RTClib RTC;
DateTime now;

void setup()
{
  Serial.begin(9600);   // open serial over USB
  Wire.begin();
  pinMode(13, OUTPUT);
  // define LED pins as outputs
  pinMode( blueLED, OUTPUT );
  pinMode( yellowLED, OUTPUT );
  pinMode( greenLED, OUTPUT );
  bool sd = false;
  while (!sd) {
    Serial.print("Initializing SD card...");
    if (SD.begin(4)) {
      sd = true;
      Serial.println("initialization done.");
    } else {
      Serial.println("initialization failed!");
      delay(1000);
    }
  }
  
  //Open up the config file and read in the data

  configfile = SD.open("Config.txt", FILE_READ);
  if (configfile) {
    recvWithEndMarker();
    waterlow = getConfigValue();
    recvWithEndMarker();
    templow = getConfigValue();
    recvWithEndMarker();
    temphigh = getConfigValue();
    recvWithEndMarker();
    lightAVGlow = getConfigValue();
    recvWithEndMarker();
    lightAVGhigh = getConfigValue();
    recvWithEndMarker();
    readingFreq = getConfigValue();
    if (readingFreq > 4) {
      Serial.println("Maxium reading perhour is 4");
      readingFreq = 4;
    }
    desiredSize = 24 * readingFreq;
    configfile.close();
  } else {
    Serial.println("error opening configfile.txt");
  }
}



void loop()
{
  now = RTC.now();
  //Get readings and print to serial
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.year());
  Serial.print("-");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
  Serial.print("Soil Moisture = ");
  //get soil moisture value from the function below and print it
  Serial.println(readSoil());
  Serial.print("Light Level = ");
  //get light value from the function below and print it
  Serial.println(readLight());
  Serial.print("Tempature = ");
  //get light value from the function below and print it
  Serial.println(readTemp());
  Log(); //Call the function to log
 //findAverages();
  Health(); // Call the function to check plant health
  if (blue == 1) {
    digitalWrite(blueLED, HIGH);
  } else {
    digitalWrite(blueLED, LOW);
  }
  if (yellow == 1) {
    digitalWrite(yellowLED, HIGH);
  } else {
    digitalWrite(yellowLED, LOW);
  }
  if (yellow == 0) {
    digitalWrite(greenLED, HIGH);
  } else {
    digitalWrite(greenLED, LOW);
  }
  delay((60 / readingFreq) * 60000);

  //This 1 second timefrme is used so you can test the sensor and see it change in real-time.
  //For in-plant applications, you will want to take readings much less frequently.
}
//This is a function used to get the soil moisture content
int readSoil()
{
  soilval = analogRead(soilPin);//Read the SIG value form sensor
  if (soilval > 1020) {
    soilval = 0;
  } else if (soilval < 400) {
    soilval = 99;
  } else {
    soilval = 164 - (0.16 * soilval);
  }
  return soilval;
}
//Get light level
int readLight()
{
  lightval = analogRead(lightPin);//Read the SIG value form sensor
  if (lightval < 150) {
    lightval = 99;
  } else if (lightval > 900) {
    lightval = 0;
  } else {
    lightval = 120 - (0.133333 * lightval);
  }
  return lightval;
}
//Get Temp
int readTemp()
{
  tempval = analogRead(tempPin);//Read the SIG value form sensor
  if (tempval > 497) {
    tempval = 0;
  } else if (tempval < 15) {
    tempval = 99;
  } else {
    tempval = 86.67 - (0.11 * tempval);
  }
  return tempval;
}
//Check values and changes LEDs
void Health() {
  if (soilval > waterlow) {
    blue = 0;
  } else {
    Serial.print("Setting blue LED because:");
    Serial.print(soilval);
    Serial.print("<");
    Serial.println(waterlow);
    blue = 1;
  }
  if (tempAVG > temphigh || tempAVG < templow || lightAVG < lightAVGlow || lightAVG > lightAVGhigh) {
    if (tempAVG >= 0 && lightAVG >= 0) {
      yellow = 1;
      Serial.println("Setting yellow LED, values:");
    Serial.println(tempAVG);
    Serial.println(lightAVG);
    }
  } else {
    yellow = 0;
  }
}

//Log
void Log() {
  logfile = SD.open("Logfile.txt", FILE_WRITE);
  if (logfile) {
    now = RTC.now();
    logfile.print(now.month());
    logfile.print("/");
    logfile.print(now.day());
    logfile.print("/");
    logfile.print(now.year());
    logfile.print("-");
    logfile.print(now.hour());
    logfile.print(":");
    logfile.print(now.minute());
    logfile.print(":");
    logfile.print(now.second());
    logfile.print(",");
    logfile.print(soilval);
    logfile.print(",");
    logfile.print(lightval);
    logfile.print(",");
    logfile.print(tempval);
    logfile.println();
    logfile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Logfile.txt");
  }
}


void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

int getConfigValue() {
  int index = findIndexOfStart() + 2;
  return ( 10 * (receivedChars[index] - '0' ) ) + receivedChars[index + 1] - '0';
}
int findIndexOfStart() {
  for (int i = 0; i < 32; i++) {
    if (receivedChars[i] == '=') {
      return i;
    }
  }
  return -1;
}
void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  bool endOfline = false;

  while (configfile.available() && endOfline == false) {
    rc = configfile.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= 32) {
        ndx = 32 - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      endOfline = true;
    }
  }
}
void findAverages() {
  int numOfLines = findNumOfLines();
  if (numOfLines < desiredSize) {
    return;
  }
  if (numOfLines > 20000) {
    Serial.println("Setting Yellow LED because log is getting to big");
    digitalWrite(yellowLED, HIGH);
    while (1);
  }
  int startLine = numOfLines - desiredSize;
  readValues(startLine);
  tempAVG = tempAVG / desiredSize;
  lightAVG = lightAVG / desiredSize;
  return;
}

int findIndexOfLight() {
  int commaCount = 0;
  for (int i = 0; i < 32; i++) {
    if (receivedChars[i] == ',') {
      commaCount++;
      if (commaCount == 2) {
        return i;
      }
    }
  }
}
int findNumOfLines() {
  logfile = SD.open("Logfile.txt", FILE_READ);
  if (logfile) {
    int lineCount = 0;
    while (logfile.available()) {
      readLine();
      lineCount++;
    }
    logfile.close();
    return lineCount;
  }
  return -1;
}

void readValues(int startLine) {
  int currentLine = 0;
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  bool endOfline = false;
  logfile = SD.open("Logfile.txt", FILE_READ);
  if (logfile) {
    while (logfile.available() && currentLine < startLine) {
      while (!endOfline) {
        rc = logfile.read();
        if (rc == endMarker) {
          endOfline = true;
        }
      }
      currentLine++;
      endOfline = false;
    }
    while (logfile.available()) {
      while (!endOfline) {
        rc = logfile.read();
        if (rc != endMarker) {
          receivedChars[ndx] = rc;
          ndx++;
          if (ndx >= 32) {
            ndx = 32 - 1;
          }
        }
        else {
          receivedChars[ndx] = '\0'; // terminate the string
          ndx = 0;
          endOfline = true;
        }
      }
      endOfline = false;
      ndx = 0;
      //extract values
      extractValues();
    }
    logfile.close();
  }
  else {
    Serial.println("Couldn't open logfile for getlineN func");
  }
}

void extractValues() {
  int index = findIndexOfLight() + 1;
  float num = (10 * (receivedChars[index] - '0')) + receivedChars[index + 1] - '0';
  float num2 = (10 * (receivedChars[index + 3] - '0')) + receivedChars[index + 4] - '0';
  tempAVG += num2;
  lightAVG += num;
}


void readLine() {
  char endMarker = '\n';
  char rc;

  while (logfile.available()) {
    rc = logfile.read();
    if (rc == endMarker) {
      return;
    }
  }
}
//end sketch
