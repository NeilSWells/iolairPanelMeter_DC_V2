# iolairPanelMeter_DC_V2
 
This is an updated version of the iolairPanelMeter project.

![IMG-9852](https://user-images.githubusercontent.com/24658072/206868530-d4637f50-6a49-4300-b112-2fa98e5db966.jpg)

![IMG-9844](https://user-images.githubusercontent.com/24658072/206868537-55dc6386-bda2-4126-8703-08ebf3b0759b.jpg)

The hardware design has been incorporated onto a single PCB.

More of the components are now SMD and added at the factory.

A further 28% improvement in energy efficiency.

A ten position rotary switch selects the function of the board.

A temperature sensor is included near the green connector to sense excessive heat rising from components below. If the temperature exceeds 45C (113F) the display will alternate between it's selected function and a superbright temperature reading. You can select whether it shows C or F.

The software has been rewritten. The previous version encapsulated the functions in a library to allow tinkering with the software. This version handles all the functions of the board in a single file.

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

Functions can only be changed before the board is powered up. Once running, the switch is ignored.

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
These functions do two things. Firstly it stores the preferred temperature scale in the EEPROM. Secondly it displays the temperature in the selected scale. Power on the board with the switch in position 8 and it will show excessive temperatures in Fahrenheit every time it is powered up. Power on in position 9 for Centigrade.

# Parts

The only parts needed on the V2 boards are

## Display driver

HT16K33 or HT16k33A or VK16k33 all SOP 24 versions. 

https://www.aliexpress.com/item/1005004738423851.html?spm=a2g0o.order_list.order_list_main.10.21ef1802Wdn0Ps

Note that the chip is rotated 180 compared to the V1 boards with the pin 1 dot at the bottom.

## Seven segment display

4 bit common cathode

https://www.aliexpress.com/item/4000279476347.html?spm=a2g0o.order_list.order_list_main.4.21ef18025l8vlf

## Connector

Connector - MSTBA 2.5/ 6-G-5.08

https://www.digikey.com/en/products/detail/phoenix-contact/1757284/260478

https://uk.farnell.com/camdenboss/ctbp9358-6/terminal-block-header-6pos-r-a/dp/2527815

## Display cable

FFC Cable, 6 Core, 1 mm pitch, Same Sided Contacts, 50 mm long

https://uk.farnell.com/multicomp-pro/mp-ffca10060503a/ffc-cord-6p-same-side-50mm-wht/dp/3385304

https://www.aliexpress.com/item/4000022055649.html?spm=a2g0o.productlist.0.0.4fcd5dbbpDS8Ir&algo_pvid=ac591bfa-7745-4d5f-8237-7b0a295e2733&algo_exp_id=ac591bfa-7745-4d5f-8237-7b0a295e2733-3&pdp_ext_f=%7B%22sku_id%22%3A%2210000000050434747%22%7D&pdp_npi=2%40dis%21GBP%210.84%210.74%21%21%21%21%21%402100bdcf16706960747111214e45c4%2110000000050434747%21sea&curPageLogUid=sdRRuynVVI8s

In case anyone is wondering why it's a 6 core cable when we only need 4 cores, JLC only had the 6 pin SMD connectors in stock.

# Power supply

To test for any issues with electrical noise, I hooked the displays up in turn to an elderly 8Ah AGM battery to provide a clean 12v power source. The oscilloscope was set to capture the original Chi Tai meter and then the scales were kept the same to compare it to both the V1 and V2 boards.

Chi Tai ammeter

![ChiTai Ammeter](https://user-images.githubusercontent.com/24658072/206869931-19022365-6b20-423e-91ee-c40b9c6e9a0e.png)

Version 2 board


![Morsun PSU](https://user-images.githubusercontent.com/24658072/206869935-5ae50a4d-cb0f-4b18-8a6a-baa82f17af71.png)

Version 1 board


![Recom PSU](https://user-images.githubusercontent.com/24658072/206869938-0c889f8c-46c9-4428-af36-8074fc300291.png)


