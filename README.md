# iolairPanelMeter_DC_V2

The project has been developed in to a commercially available product
# Completed AC and DC boards are availalbe at https://nordybits.com/

This is an updated version of the iolairPanelMeter project.

![IMG-9852](https://user-images.githubusercontent.com/24658072/206868530-d4637f50-6a49-4300-b112-2fa98e5db966.jpg)

![IMG-9844](https://user-images.githubusercontent.com/24658072/206868537-55dc6386-bda2-4126-8703-08ebf3b0759b.jpg)

# Files

A new release called iolairPanelMeter_DC_V2_SOP28 is available - click the link on the right hand panel. The only change is that it uses the more easily availible SOP28 version of the HT16K33 display chip.

Panel_Meter_DC_V2.ino - The Arduino software

Panel_Meter_DC_V2.zip - KiCad V6 design files

gerbers.zip - JLC formatted manufacturing files

bom.csv and positions.csv - JLC formatted SMD parts and placement files

# Changes

The hardware design has been incorporated onto a single PCB.

More of the components are now SMD and added at the factory.

A 30% reduction in power consumption.

Reduced electrical noise.

A ten position rotary switch selects the function of the board.

A temperature sensor is included near the green connector to sense excessive heat rising from breakers and bus bars below. If the temperature exceeds 45C (113F) the display will alternate between it's selected function and a superbright temperature reading. You can select whether it shows C or F.

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
These functions do two things. Firstly it stores the preferred temperature scale in the EEPROM. Secondly it displays the current temperature in the selected scale. Power on the board with the switch in position 8 once and if the temperature exceeds 45C (113F) it will display the warning temperature in Fahrenheit every time it is powered up regardless of what function is active. Power on once in position 9 for Centigrade.

# Parts

The only parts needed on the V2 boards are

## Display driver

HT16K33 or HT16k33A or VK16k33. There are two versions of this design, one for the SOP24 version and one for the more common and cheaper SOP28. Future designs will stick to SOP28

https://www.aliexpress.com/item/1005004738423851.html?spm=a2g0o.order_list.order_list_main.10.21ef1802Wdn0Ps

Note that the chip is rotated 180 compared to the V1 boards with the pin 1 dot at the bottom. This will allow the design to be easily adapted to the SOP 28 version if it comes in stock at JLC. A factory installed display chip would make the project very simple to build.

I tried ordering the minimum quantity of 458 chips to see if they would stock them but to no avail:
https://jlcpcb.com/partdetail/Holtek-HT16K33A/C5205198

## Seven segment display

4 bit common cathode

https://www.aliexpress.com/item/4000279476347.html?spm=a2g0o.order_list.order_list_main.4.21ef18025l8vlf

## Connector

Connector - MSTBA 2.5/ 6-G-5.08

https://www.digikey.com/en/products/detail/phoenix-contact/1757284/260478

https://uk.farnell.com/camdenboss/ctbp9358-6/terminal-block-header-6pos-r-a/dp/2527815

## Display cable

FFC Cable, 6 Core, 1 mm pitch, Same Sided Contacts, 50 mm long

Note that the cable is installed with the contacts facing up. Pull the black part of the connector forward from the white part to open, insert the cable (about 1mm of contact should remain visible when the cable is fully inserted) click it home to lock the cable in place.

https://uk.farnell.com/multicomp-pro/mp-ffca10060503a/ffc-cord-6p-same-side-50mm-wht/dp/3385304

https://www.aliexpress.com/item/4000022055649.html?spm=a2g0o.productlist.0.0.4fcd5dbbpDS8Ir&algo_pvid=ac591bfa-7745-4d5f-8237-7b0a295e2733&algo_exp_id=ac591bfa-7745-4d5f-8237-7b0a295e2733-3&pdp_ext_f=%7B%22sku_id%22%3A%2210000000050434747%22%7D&pdp_npi=2%40dis%21GBP%210.84%210.74%21%21%21%21%21%402100bdcf16706960747111214e45c4%2110000000050434747%21sea&curPageLogUid=sdRRuynVVI8s

In case anyone is wondering why it's a 6 core cable when we only need 4 cores, JLC only had the 6 pin SMD connectors in stock.

# Programming

How the board is programmed is the same as the original iolairPanelMeter project.

Note that the library used in Version 1 is not compatible with Version 2. The wiring betweeen the display chip and the display is slightly different.

# Manufacture

Manufacture is very similar to the original, the only thing to note is that JLC consider it to be a board with 2 different designs. This is selected under PCB Specification - they will notice and make a surcharge if you get this wrong as I did. The maufacturing files and the SMD files are in Gerbers.zip

There is no point in ordering a soldering stencil with this version as there are other factory fitted components close by on the board.

# Assembly

This YouTube video is a good example of the technique I used to solder the display chip. Putting too much solder on with the syringe can be a problem - its better to put on too little and then add a little more if a pin is dry. Check for solder bridges between the pins when you're done.

https://www.youtube.com/watch?v=rgcupYOnvBo


# Power supply

## Power consumption
```
Voltage     13.8V   27.6V

ChaTai      2.50W   2.50W
Version 1   0.19W   0.33W
Version 2   0.14W   0.22W
```

Chi Tai quote 2.5w but when I measured 3 old meters they consumed 2.2W 2.6W and 3.0W

The DC-DC converters on the V1 and V2 boards are not quite as efficient at 27.6v

## Heat

On all of my old Chi Tai meters, the bulk of the energy is wasted in the power supply part of the board. In the next two images, you can see that the heat being created in the back right hand side of the board. The six screw heads of the green connector are just visible near the top of the images. On a working board, the hottest component was sitting at 129°C (264°F). On a failed DC ammeter, I measured 150°C (302°F). These temperatures were with the boards in free air, sitting contained in a small case, it can only be hotter.

This explains the melted plastic on some of my old cases.

![20221218T130954](https://user-images.githubusercontent.com/24658072/208310250-b84dac46-41a1-4198-9457-aa668e88df05.JPG)

![20221218T131006](https://user-images.githubusercontent.com/24658072/208310258-b6d5dd84-0116-43ca-a2ba-638512e4fc36.JPG)

# 

By comparison, this is a V2 board after an hour. The hottest component is the CPU which is sitting at 7°C (12°F) above ambient

![20221218T161002](https://user-images.githubusercontent.com/24658072/208310263-d73ecab6-d3b3-4dea-ad6c-afebd9159968.JPG)

## Electrical noise

To test for any issues with electrical noise, I hooked the displays up in turn to an elderly 8Ah AGM battery to provide a clean 12v power source. The oscilloscope was set to capture the effect on the 12v source of the original Chi Tai meter. The scales were then kept the same to compare it to both the V1 and V2 boards.

Chi Tai ammeter

![ChiTai Ammeter](https://user-images.githubusercontent.com/24658072/206869931-19022365-6b20-423e-91ee-c40b9c6e9a0e.png)

# 
Version 1 board

![Recom PSU](https://user-images.githubusercontent.com/24658072/206869938-0c889f8c-46c9-4428-af36-8074fc300291.png)

# 
Version 2 board

![Morsun PSU](https://user-images.githubusercontent.com/24658072/206869935-5ae50a4d-cb0f-4b18-8a6a-baa82f17af71.png)

# Schematic

![image](https://user-images.githubusercontent.com/24658072/206873892-3742208f-4323-4dca-906c-5b88e24d7d32.png)

## Contact
If you have any questions or suggestions, email NeilSWells@gmail.com

The use of this design is entirely at your own risk.

## License

MIT

# Coming Soon

AC Voltmeter

![Panel Meter AC Volts front](https://user-images.githubusercontent.com/24658072/206923200-b0f1550e-4ad4-45c8-82a3-7cddd20a23de.png)

![Panel Meter AC Volts back](https://user-images.githubusercontent.com/24658072/206922539-6869c05a-bbcc-480c-87f0-595a70e082ff.png)


![image](https://user-images.githubusercontent.com/24658072/206922061-1b29ce6f-0ab2-40f5-b56c-80853dbcdf10.png)
