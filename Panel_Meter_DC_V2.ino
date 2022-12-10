#include <Wire.h>
#include <avr/wdt.h>

//Change to suit
#define CALIBRATION_VOLTS 80
#define CALIBRATION_300A 666
#define CALIBRATION_400A 500
#define CALIBRATION_500A 400
#define CALIBRATION_800A 250
#define CALIBRATION_1000A 200

#define BOUNDS_TEMPERATURE 45 //Alarm temperature in centigrade
#define BOUNDS_12V_LOW 122
#define BOUNDS_12V_HIGH 148
#define BOUNDS_24V_LOW 244
#define BOUNDS_24V_HIGH 296

#define BRIGHTNESS_LOW 1 //Min 0
#define BRIGHTNESS_HIGH 15 //Max 15

//Do not change
#define ADDRESS_DISPLAY 0x70
#define ADDRESS_INA226 0x40
#define ADDRESS_TMP75 0x48
#define DISPLAY_COMMAND_BRIGHTNESS 0xE0

uint8_t displayCharacter[4];
uint8_t displayDigit[3];
uint8_t displayBrightness;
uint16_t calibrationFactor;
uint8_t function = 0;
int32_t result = 0;
uint16_t boundsLow = 0;
uint16_t boundsHigh = 0;
bool displayFarenheit = false;

static const uint8_t displaySegmentMap[] =
{
    //The bits refer to the LEDs of a seven segment display.
    //Decimal point, middle bar, top left, bottlom left,  bottom bar, bottom right, top right, top bar
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111100, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01100111, // 9
    0b00000000, // Blank - 10
    0b01000000, // Minus - 11
    0b01110111, //A - 12
    0b01011000, //c - 13
    0b01111001, //E - 14
    0b01110001, //F - 15
    0b01110011, //P - 16
    0b01010000, //r - 17
    0b01101101, //S - 18
    0b01111000, //t - 19
    0b00011100, //u - 20
    0b11111111 //8. - 21
};


void setup()
{
    Wire.begin();
    BeginDisplay();
    BeginSwitch();
    SetFunction();
    displayFarenheit = EepromRead();
    BeginWatchdog(); //Set an 8 second watchdog timer to reboot the panel if wdt_reset() isn't called.
}


void loop()
{
    switch (function)
    {
        case 0:
            FunctionTest();
            break;
        case 1:
            FunctionVolts();
            break;
        case 2:
            FunctionVolts();
            break;
        case 3:
            FunctionAmps();
            break;
        case 4:
            FunctionAmps();
            break;
        case 5:
            FunctionAmps();
            break;
        case 6:
            FunctionAmps();
            break;
        case 7:
            FunctionAmps();
            break;
        case 8:
            FunctionTemperature(true); //Show farenheit
            break;
        case 9:
            FunctionTemperature(false); //Show centigrade
            break;
    }
    CheckTemperature();
    wdt_reset(); //Reset the watchdog timer so that the board doesn't reboot itself
}


////////////////////////////////////////////////////////////
// Setup
////////////////////////////////////////////////////////////

void SetFunction()
{
    function = ReadSwitchValue();
    switch (function)
    {
        case 0:
            DisplaySequence(19, 14, 18, 19); //tESt
            break;
        case 1:
            DisplaySequence(10, 1, 2, 20); //12v
            boundsLow = BOUNDS_12V_LOW;
            boundsHigh = BOUNDS_12V_HIGH;
            break;
        case 2:
            DisplaySequence(10, 2, 4, 20); //24v
            boundsLow = BOUNDS_24V_LOW;
            boundsHigh = BOUNDS_24V_HIGH;
            break;
        case 3:
            DisplaySequence(3, 0, 0, 12); //300A
            calibrationFactor = CALIBRATION_300A;
            break;
        case 4:
            DisplaySequence(4, 0, 0, 12); //400A
            calibrationFactor = CALIBRATION_400A;
            break;
        case 5:
            DisplaySequence(5, 0, 0, 12); //500A
            calibrationFactor = CALIBRATION_500A;
            break;
        case 6:
            DisplaySequence(8, 0, 0, 12); //800A
            calibrationFactor = CALIBRATION_800A;
            break;
        case 7:
            DisplaySequence(1, 0, 0, 0); //1000
            calibrationFactor = CALIBRATION_1000A;
            break;
        case 8:
            DisplaySequence(19, 14, 10, 15);
            if (!EepromRead ()) EepromWrite (true); //If the farenheit bit is not set in the Eeprom, set it.
            break;
        case 9:
            DisplaySequence(19, 14, 10, 13);
            if (EepromRead ()) EepromWrite (false); //If the farenheit bit is set in the Eeprom, clear it.
            break;
    }
    delay(1000);
}


void BeginDisplay()
{
    delay(50);
    SendCommand(ADDRESS_DISPLAY, 0b00100001); //Turn on the display system oscillator
    SendCommand(ADDRESS_DISPLAY, 0b10000001); //Set display on, blinking off
    Clear();
    SetBrightness(BRIGHTNESS_LOW);
}


void BeginSwitch()
{
    pinMode(4, OUTPUT); //Switch Pad Common - Processor Pad 2 - Arduino Pin 4
    pinMode(5, INPUT_PULLUP); //Switch Pad 1 - Processor Pad 9 - Arduino Pin 5
    pinMode(3, INPUT_PULLUP); //Switch Pad 2 - Processor Pad 1 - Arduino Pin 3
    pinMode(2, INPUT_PULLUP); //Switch Pad 4 - Processor Pad 32 - Arduino Pin 2
    pinMode(6, INPUT_PULLUP); //Switch Pad 8 - Processor Pad 10 - Arduino Pin 6
}


////////////////////////////////////////////////////////////
// Panel functions
////////////////////////////////////////////////////////////

void FunctionTest()
{
    SetBrightness(15);
    DisplaySequence(21, 21, 21, 21); //8.8.8.8.
    delay(2000);
    wdt_reset();
    result = GetTemperature();
    DisplayTemperature(result, false);
    delay(2000);
    wdt_reset();
    result = GetTemperature();
    DisplayTemperature(result, true);
    delay(2000);
    wdt_reset(); // Last watchdog reset, the next pair of loops will take approximately 8 seconds to run
    for (int8_t i = 0; i < 10; i++)
    {
        DisplayInteger(i * 111, false);
        SetBrightness(i);
        delay(350);
    }
    SetBrightness(15);
    for (int8_t i = -9; i < 1; i++)
    {
        DisplayInteger(i * 111, true);
        delay(350);
    }
    delay(1000);
    DisplaySequence(0, 0, 16, 18); //OOPS the watchdog timer should have rebooted the panel by now
    for (;;); //This is an infinte loop. If the watchdog timer is not working, we will be stuck here forever.
}


void FunctionVolts()
{
    result = GetDcVolts();
    DisplayInteger(result, true);
    if ((result > boundsLow) && (result < boundsHigh)) SetBrightness(BRIGHTNESS_LOW);
    else SetBrightness(BRIGHTNESS_HIGH);
    delay(323);
}


void FunctionAmps()
{
    result = GetDcAmps();
    DisplayInteger(result, false);
    delay(323);
}


void FunctionTemperature(bool showFarenheit)
{
    result = GetTemperature();
    DisplayTemperature(result, showFarenheit);
    delay(333);
}

////////////////////////////////////////////////////////////
// Get readings from the hardware
////////////////////////////////////////////////////////////

int32_t GetDcVolts()
{
    int32_t r = 0;
    SendConfigCommand(ADDRESS_INA226, 0x00, 0b01000110, 0b00110110);  //https://www.ti.com/lit/ds/symlink/ina226.pdf Averages = 64, Shunt Voltage Conversion Time = 4.156ms, Operating Mode = Bus Voltage Continuous
    delay(5);
    SendCommand(ADDRESS_INA226, 0x02); //Get the voltage reading
    delay(5);
    r = ReceiveResult(ADDRESS_INA226);
    r /= CALIBRATION_VOLTS;
    return (r);
}


int32_t GetDcAmps()
{
    float r = 0; //Result from the ADC
    SendConfigCommand(ADDRESS_INA226, 0x00, 0b01000110, 0b00110101);  //https://www.ti.com/lit/ds/symlink/ina226.pdf Averages = 64, Shunt Voltage Conversion Time = 4.156ms, Operating Mode = Shunt Voltage Continuous
    delay(5);
    SendCommand(ADDRESS_INA226, 0x01); //Deliver the Shunt Voltage Register reading
    delay(5);
    r = ReceiveResult(ADDRESS_INA226);
    r *= 10; //Multiply the reading by 10 to allow the calibration factor to be more precise
    r /= calibrationFactor;
    return ((int32_t)r);
}


int32_t GetTemperature()
{

    Wire.beginTransmission(ADDRESS_TMP75);  //https://www.ti.com/lit/ds/symlink/tmp75.pdf?ts=1670515844253&ref_url=https%253A%252F%252Fwww.google.com%252F
    Wire.write(0b00000001); //Configuration register
    Wire.write(0b10011011); // Set One Shot on, 2 x Resolution bits to 0.5c, 2 x Fault bits - don't care, Polarity - don't care, Thermostat Mode - don't care, Shutdown Mode on
    Wire.endTransmission();
    Wire.beginTransmission(ADDRESS_TMP75);
    Wire.write(0b00000000); //Temperature register
    Wire.endTransmission();
    return ReceiveResult(ADDRESS_TMP75) / 256;
}


void CheckTemperature()
{
    if ( GetTemperature() > BOUNDS_TEMPERATURE)
    {
        Clear();
        delay(200);
        SetBrightness(BRIGHTNESS_HIGH);
        FunctionTemperature(displayFarenheit);
        delay(333);
        SetBrightness(BRIGHTNESS_LOW);
        Clear();
        delay(200);
    }
}


uint8_t ReadSwitchValue()
{
    uint8_t readingFirst = 0;
    uint8_t readingSecond = 1;
    digitalWrite(4, LOW);
    delay(1);
    do
    {
        readingFirst = ((!digitalRead(6) * 8) + (!digitalRead(2) * 4) + (!digitalRead(3) * 2) + (!digitalRead(5)));
        delay(100);
        readingSecond = ((!digitalRead(6) * 8) + (!digitalRead(2) * 4) + (!digitalRead(3) * 2) + (!digitalRead(5)));
    }
    while (readingFirst != readingSecond);
    digitalWrite(4, HIGH);
    return readingFirst;
}

////////////////////////////////////////////////////////////
// Display functions
////////////////////////////////////////////////////////////

void SetBrightness(uint8_t brightness)
{
    if (brightness != displayBrightness)
    {
        if (brightness > 15) brightness = 15;
        SendCommand(ADDRESS_DISPLAY, DISPLAY_COMMAND_BRIGHTNESS | brightness);
        displayBrightness = brightness;
    }
}


void DisplayInteger(int32_t number, bool showDecimalPoint)
{
    //Ensure the number is within bounds
    if ((number < -999) || (number > 999))
    {
        DisplayError();
        return;
    }
    //If the number is less than zero, set the first display character to the minus sign and invert the number for further calculations
    if (number < 0)
    {
        displayCharacter[0] = displaySegmentMap[11]; //Minus
        number *= -1;
    }
    else
    {
        displayCharacter[0] = displaySegmentMap[10]; //Space
    }
    //Split out the digits of the number
    displayDigit[0] = (number / 100);
    displayDigit[1] = (number / 10) % 10;
    displayDigit[2] = (number % 10);
    //Set each character to the corresponding value in the character map
    displayCharacter[1] = displaySegmentMap[displayDigit[0]];
    if (showDecimalPoint)
    {
        displayCharacter[2] = displaySegmentMap[displayDigit[1]] | 0b10000000;
    }
    else
    {
        displayCharacter[2] = displaySegmentMap[displayDigit[1]];
    }
    displayCharacter[3] = displaySegmentMap[displayDigit[2]];
    SendCharacters();
}


void DisplayTemperature(int32_t temperature, bool showFarenheit)
{
    //Ensure the number is within bounds
    if (showFarenheit)
    {
        temperature = temperature * 9;
        temperature = temperature / 5;
        temperature = temperature + 32;
    }
    if ((temperature < -99) || (temperature > 999))
    {
        DisplayError();
        return;
    }
    //If the number is less than zero, set the first display character to the minus sign and invert the number for further calculations
    if (temperature < 0)
    {
        temperature *= -1;
        //Split out the digits of the number
        displayDigit[0] = (temperature / 10) % 10;
        displayDigit[1] = (temperature % 10);
        displayCharacter[0] = displaySegmentMap[11]; //Minus
        displayCharacter[1] = displaySegmentMap[displayDigit[0]];
        displayCharacter[2] = displaySegmentMap[displayDigit[1]];
    }
    else
    {
        //Split out the digits of the number
        displayDigit[0] = (temperature / 100);
        displayDigit[1] = (temperature / 10) % 10;
        displayDigit[2] = (temperature % 10);
        if (displayDigit[0] > 0)
        {
            displayCharacter[0] = displaySegmentMap[displayDigit[0]];
        }
        else
        {
            displayCharacter[0] = displaySegmentMap[10];
        }
        displayCharacter[1] = displaySegmentMap[displayDigit[1]];
        displayCharacter[2] = displaySegmentMap[displayDigit[2]];
    }
    if (showFarenheit)
    {
        displayCharacter[3] = displaySegmentMap[15]; //Show the F
    }
    else
    {
        displayCharacter[3] = displaySegmentMap[13]; //Show the c
    }
    SendCharacters();
}


void DisplayError()
{
    DisplaySequence(10, 14, 17, 17);
}


void DisplaySequence(uint8_t charA, uint8_t charB, uint8_t charC, uint8_t charD)
{
    displayCharacter[0] = displaySegmentMap[charA];
    displayCharacter[1] = displaySegmentMap[charB];
    displayCharacter[2] = displaySegmentMap[charC];
    displayCharacter[3] = displaySegmentMap[charD];
    SendCharacters();
}


void SendCharacters()
{
    //Send all the characters to the display
    Wire.beginTransmission(ADDRESS_DISPLAY);
    Wire.write(0);
    Wire.write(displayCharacter[3]);
    Wire.write(2);
    Wire.write(displayCharacter[2]);
    Wire.write(4);
    Wire.write(displayCharacter[1]);
    Wire.write(6);
    Wire.write(displayCharacter[0]);
    Wire.endTransmission();
}


void Clear()
{
    DisplaySequence(10, 10, 10, 10);
}

////////////////////////////////////////////////////////////
// Communicate with I2C hardware
////////////////////////////////////////////////////////////

void SendCommand (uint8_t address, uint8_t command)
{
    Wire.beginTransmission(address);
    Wire.write(command);
    Wire.endTransmission();
}


void SendConfigCommand (uint8_t address, uint8_t registerAddress, uint8_t valueA, uint8_t valueB)
{
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.write(valueA);
    Wire.write(valueB);
    Wire.endTransmission();
}


int16_t ReceiveResult(uint8_t address)
{
    Wire.requestFrom(address, (uint8_t)2); //Read the two byte result
    return ((Wire.read() << 8) | Wire.read()); //Get the two byte reading
}

////////////////////////////////////////////////////////////
// EEPROM
////////////////////////////////////////////////////////////

//Store a single bit in address 1 of the Eeprom to set Centigrade or Farenheit readings
void EepromWrite (bool setting)
{
    while (EECR & (1 << EEPE)); // Wait for completion of previous write
    EEAR = 0x01; // Set the Eeprom address
    if (setting) EEDR = 0b00000001; //Set the data bit
    else EEDR = 0b00000000;
    EECR |= (1 << EEMPE); //Write logical one to EEMPE
    EECR |= (1 << EEPE); //Start eeprom write by setting EEPE
}


bool EepromRead ()
{
    while (EECR & (1 << EEPE)); // Wait for completion of previous write
    EEAR = 0x01; // Set the Eeprom address
    EECR |= (1 << EERE); //Start eeprom read by writing EERE
    return (EEDR == 0b00000001); //Return data from Data Register
}

////////////////////////////////////////////////////////////
// Watchdog timer
////////////////////////////////////////////////////////////

//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

void BeginWatchdog()
{
    //Reboot the device if it hangs for 8 seconds
    cli(); //Disable interrupts
    wdt_reset();
    WDTCSR |= 0b00011000; //Watchdog configuration mode, set WDCE and WDE to 1
    WDTCSR = 0b00101001; // Watchdog settings - 8 seconds to reset, WDIF 0, WDIE 0, WDP3 1, WDCE 0, WDE 1, WDP2 0, WDP1 0, WDP0 1
    sei(); //Enable interrupts
}
