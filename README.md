# GardenHelper
Code for Arduino GardenHelper


To Do Hardware:
Calibrate Sensors
Read values from Config file
Implement RTC
Critical Water low vs Non critical water low
Convert from breadboard to prototype

To Do Software:
GUI
Config file
CSV data analysing 

Bugs:
First log entry always reads with zero date

Example Config.txt
int waterlow = 0-100; // 0-100 %, If moist sensor goes below this value, trigger blue LED
int waterhigh = 0-100; // 0-100%
int templow = 0-40; // If temp sensor goes below this Celeius value, trigglr yellow LED
int temphigh = 0-40; //If temp sensor goes below this Celeius value, trigglr yellow LED
int lightAVGlow = 0-100; //0-100%
int lightAVGhigh = 0-100; //0-100%
int readingFreq = 0; // Specific in minutes 

