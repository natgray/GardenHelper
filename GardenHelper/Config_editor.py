#Writes the user defined config values to a config file for garden helper
#Author: Angelo Calitis

# int waterlow = 0-100; // 0-100 %, If moist sensor goes below this value, trigger blue LED
# int waterhigh = 0-100; // 0-100%
# int templow = 0-40; // If temp sensor goes below this Celeius value, trigglr yellow LED
# int temphigh = 0-40; //If temp sensor goes above this Celeius value, trigglr yellow LED
# int lightAVGlow = 0-100; //0-100%
# int lightAVGhigh = 0-100; //0-100%
# int readingFreq = 0; // Specific in minutes

# Open the config file
with open('config.txt', 'r') as file:
    # read a list of lines into data
    config = file.readlines()

# Ask for user's input
waterlow = input('Enter lowest water % before triggering water sensor: ')

# Change the appropriate line of the file, e.g. config[0] changes 1st line of config.txt
# \n is required
config[0] = str(waterlow) + ' \n'

templow = input('Enter lowest Celsius temperature before triggering coldness warning sensor: ')
config[2] = str(templow) + ' \n'

temphigh = input('Enter highest Celsius temperature before triggering heat warning sensor: ')
config[2] = str(temphigh) + ' \n'

# writing everything back
with open('stats.txt', 'w') as file:
    file.writelines(config)