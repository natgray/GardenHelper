#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

int soilval = 0; //value for storing moisture value 
int lightval = 0; //value for storing light level
int tempval = 0; //value for storing temp level
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int lightPin = A1; //Declare a variable for the light sensor
int tempPin = A2; //Declare a variable for the temp sensor
int readingNum = 0; //Counter
// LED pins
int blueLED = 5;
int yellowLED = 6;
int greenLED = 7;
//flags for LEDs
int green = 1;
int blue = 0;
int yellow = 0;
//default values for health check
int waterlow = 750;
int templow = 100;
int temphigh = 200;
int lightAVGlow = 400;
int lightAVGhigh = 800;
int readingFreq = 5000;
File logfile;

void setup() 
{
  Serial.begin(9600);   // open serial over USB
    while (!Serial) ; 
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");
   Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  
logfile = SD.open("Logfile.txt", FILE_WRITE);
if(logfile){
  logfile.println("Time,Moisture,Light,Temp");
logfile.close();
} else {
    // if the file didn't open, print an error:
    Serial.println("error opening Logfile.txt");
  }
// define LED pins as outputs
  pinMode( blueLED, OUTPUT ); 
  pinMode( yellowLED, OUTPUT );
  pinMode( greenLED, OUTPUT );
}


void loop() 
{
if (Serial.available()) {
    processSyncMessage();
  }
  //Get readings and print to serial
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
if(blue == 1){
digitalWrite(blueLED, HIGH);
} else {
  digitalWrite(blueLED, LOW);
}
if(yellow == 1){
digitalWrite(yellowLED, HIGH);
}
if(yellow == 0){
digitalWrite(greenLED, HIGH);
} else {
  digitalWrite(greenLED, LOW);
}
delay(readingFreq);

//This 1 second timefrme is used so you can test the sensor and see it change in real-time.
//For in-plant applications, you will want to take readings much less frequently.
delay(1000);//take a reading every second
}
//This is a function used to get the soil moisture content
int readSoil()
{
    soilval = analogRead(soilPin);//Read the SIG value form sensor 
    return soilval;//send current moisture value
}
//Get light level
int readLight()
{
    lightval = analogRead(lightPin);//Read the SIG value form sensor 
    return lightval;//send current light value
}
//Get Temp
int readTemp()
{
    tempval = analogRead(tempPin);//Read the SIG value form sensor 
    return tempval;//send current temp value
}
//Check values and changes LEDs
void Health(){
if(soilval > waterlow){
  blue = 1;
} else{
  blue = 0;
}
if(tempval > temphigh || tempval < templow || lightval < lightAVGlow || lightval > lightAVGhigh){
  yellow = 1;
}
}

//Log
void Log(){
logfile = SD.open("Logfile.txt", FILE_WRITE);
if(logfile){
  logfile.print(month());
  logfile.print("/");
  logfile.print(day());
  logfile.print("/");
  logfile.print(year());
  logfile.print("-");
  logfile.print(hour());
   logfile.print(":");
  logfile.print(minute());
   logfile.print(":");
  logfile.print(second());
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


void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}
