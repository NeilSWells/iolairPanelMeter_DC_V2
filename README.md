# iolairPanelMeter_DC_V2
Updated DC Voltmeter and ammeter design that fits inside a Chi Tai case
 
This is an updated version of the iolairPanelMeter project.

The hardware design has been incorporated onto a single PCB.

More of the components are now SMD and added at the factory.

A further 28% improvement in energy efficiency.

A ten position rotary switch selects the function of the board.

A temperature sensor is included near the green connector to sense excessive heat rising from components below. If the temperature exceeds 45C (113F) the display will alternate between it's selected function and a superbright temperature reading. You can select whether it shows C or F.

The software has been rewritten as a single file. The previous version encapsulated the functions in a library to allow tinkering with the functions. This version handles all the functions of the board.

The code includes a Watchdog function to reboot the board if it hangs. I've had three boards running on my boat for nearly two years without an issue, but it's there just in case.

The functions that can be selected are
```
0 - Test
1 - 12V DC Voltmeter
2 - 24V DC Voltmeter
3 - 300A DC Ammeter
4 - 400A DC Ammeter
5 - 500A DC Ammeter
6 - 800A DC Ammeter
7 - 1000A DC Ammeter
8 - Set Fahrenheit
9 - Set Centigrade
```

At power up, the board displays its selected function for one second (within the limits of 7 segment displays)
tESt, 12u, 24u, 300A, 400A, 500A, 800A, 1000, tE c, tE F

## Test
The code starts cycles through a number of tests including lighting all segments at full brightness, temperature in Centigrade and Fahrenheit, count up and down. At the end of the test, the code goes into an infinite loop to check that the Watchdog timer reboots the board correctly.

## 12V DC Voltmeter
Display the voltage, alerting if the reading is <= 12.2v or >= 14.8v 

## 24v DC Voltmeter
Display the voltage, alerting if the reading is <= 24.4v or >= 29.6v 

## 300A - 1000A DC Ammeter
Displays the amperage, the value should be selected to match the shunt (or match the previous meter)

## Set Fahrenheit & Set Centigrade
These functions do two things. Firstly it stores the preferred temperature scale in the EEPROM. Secondly it displays the temperature in the selected scale. Power on the board with the switch in position 8 and it will show excessive temperatures in Fahrenheit. Power on in position 9 for Centigrade.


