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
int readingNum = 0; //Counter
int* lightHistory = 0;
int* tempHistory = 0;
int arrayIndex = 0;
int arraySize = 0;
int tempAVG = 0;
int lightAVG = 0;

//default values for health check
//All values are defined as a % from 0 to 100
int waterlow = 30;
int templow = 30;
int temphigh = 80;
int lightAVGlow = 20;
int lightAVGhigh = 80;
//default reading Freq in readings per hour
int readingFreq = 1;

File logfile, configfile;
const byte numChars = 32;
char receivedChars[numChars];
RTClib RTC;
DateTime now;

void setup()
{
  Serial.begin(9600);   // open serial over USB
  Wire.begin();
  while (!Serial) ;
  pinMode(13, OUTPUT);
  // define LED pins as outputs
  pinMode( blueLED, OUTPUT );
  pinMode( yellowLED, OUTPUT );
  pinMode( greenLED, OUTPUT );

  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
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
    configfile.close();
  } else {
    Serial.println("error opening configfile.txt");
  }
  logfile = SD.open("Logfile.txt", FILE_WRITE);
  if (logfile) {
    logfile.println("Time,Moisture,Light,Temp");
    logfile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Logfile.txt");
  }
  readingFreq = 60;
 lightHistory = new int [readingFreq];
  tempHistory = new int [readingFreq];
  arraySize = readingFreq;
  for(int i = 0; i < arraySize; i++){
    lightHistory[i] = -1;
    tempHistory[i] = -1;
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
  Serial.print(now.second());
  Serial.print(",");
  Serial.print("Reading #");
  Serial.println(readingNum);
  readingNum++;
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
  delay((60/readingFreq) * 60000);

  //This 1 second timefrme is used so you can test the sensor and see it change in real-time.
  //For in-plant applications, you will want to take readings much less frequently.
}
//This is a function used to get the soil moisture content
int readSoil()
{
  soilval = analogRead(soilPin);//Read the SIG value form sensor
  Serial.print("Water Raw:");
  Serial.println(soilval);
  if(soilval>1020){
    soilval = 0;
  } else if (soilval < 400){
    soilval = 100;
  } else {
    soilval = 164 - (0.16 * soilval);
  }
  return soilval;
}
//Get light level
int readLight()
{
  lightval = analogRead(lightPin);//Read the SIG value form sensor
  if(lightval<150){
    lightval= 100;
  } else if (lightval > 900){
    lightval = 0;
  } else{
    lightval = 120-(0.133333*lightval);
  }
   return lightval;
}
//Get Temp
int readTemp()
{
  tempval = analogRead(tempPin);//Read the SIG value form sensor
  if(tempval > 497){
    tempval = 0;
  } else if (tempval < 15){
    tempval = 100;
  } else {
    tempval= 86.67 - (0.11*tempval);
  }
   return tempval;
}
//Check values and changes LEDs
void Health() {
  //Read the latest values into the array
  if(arrayIndex = arraySize){
    arrayIndex = 0;
  }
  tempHistory[arrayIndex] = readTemp();
  lightHistory[arrayIndex] = readLight();
  arrayIndex++;
  averageArrays();
  
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
    yellow = 1;
  } else {
    yellow = 0;
  }
}

 //Find average of arrays

void averageArrays(){
  int elements = 0;
  int sum = 0;
  for(int i = 0; i < arraySize; i++){
    if(tempHistory[i] != -1){
      elements++;
      sum += tempHistory[i];
    }
  }
  tempAVG = sum/elements;
  Serial.print("Avg temp");
  Serial.println(tempAVG);
  if(elements < (readingFreq*8)){
    tempAVG = 50;
  }
    elements = 0;
   sum = 0;
  for(int i = 0; i < arraySize; i++){
    if(lightHistory[i] != -1){
      elements++;
      sum += lightHistory[i];
    }
  }
  lightAVG = sum/elements;
  Serial.print("Avg light");
  Serial.println(lightAVG);
  if(elements < (readingFreq*8)){
    lightAVG = 50;
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
  for (int i = 0; i < numChars; i++) {
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
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      endOfline = true;
    }
  }
}

//end sketch
