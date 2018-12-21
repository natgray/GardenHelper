# GardenHelper
![alt text](https://github.com/natgray/GardenHelper/blob/master/GardenHelper/GardenHelperLogo.png "Garden Helper Logo")

*GardenHelper* is an arduino project aimed at assisting people with keeping house plants or small gardens healthy and happy.

## Primary features of Garden Helper:
- Light Sensor
- Soil Moisture Sensor
- Tempature Sensor
- SDCard Interface

## What can it do?
Gardenhelper will monitor Light, tempature and soil mositure values, if any of these are out of a user definied range LEDs will inform the user their plant needs some love. 

## What do the different LEDs mean?
- A Blue LED means your plant needs more water.
- A Yellow LED means that the average tempature or average light level your plant is exposed to is too high or too low.
- A Green LED means your plant is healthy and happy.

![alt text](https://github.com/natgray/GardenHelper/blob/master/GardenHelper/CircuitOverview.png "Circuit Overview")

## Component List
- Arduino UNO (Arduino Nano verison is planned)
- DS3231 AT25C32 Real Time Clock Module
- SDHC mini Micro SD Card Reader Module
- Soil Moisture Sensor
- Lumiance Light Sensor
- TMP324 Tempature Sensor
- 3 5mm LEDs (Blue, Yellow, Green)

