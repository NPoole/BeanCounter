/************************************************************************************************
 *           --------------------------------------------------------------------------------
 *            ______                     ______                     __              
 *           |   __ \.-----.---.-.-----.|      |.-----.--.--.-----.|  |_.-----.----.
 *          |   __ <|  -__|  _  |     ||   ---||  _  |  |  |     ||   _|  -__|   _|
 *         |______/|_____|___._|__|__||______||_____|_____|__|__||____|_____|__|  
 *                                                       SMT Parts Counting Tool
 *       ( )         ( )         ( )         ( )         ( )         ( )         ( )
 *      --------------------------------------------------------------------------------
 * 
 * Ver 1.0
 * Targeting ATtiny861V-10MU
 * Built in Arduino using Spence Konde's ATTinyCore (https://github.com/SpenceKonde/ATTinyCore)
 * 
 * Copyright 2022 Nick Poole 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software 
 * is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ************************************************************************************************/

#include <EEPROM.h>

// Pin defines
#define DIGIT1_CATHODE 8
#define DIGIT2_CATHODE 9
#define DIGIT3_CATHODE 10
#define IRLED 11
#define STARTBTN 12
#define SETBTN 13
#define IRSENSOR_A A9
#define IRSENSOR_B A6

// EEPROM Addresses
#define EEPROM_ADDR_PITCH 0x00
#define EEPROM_ADDR_MODE 0x01

// This is the amount of dead time where we allow a digit
// to be on before moving on to the next. Ideally, we would
// never busy-wait like this. We're already polling the
// IR sensors in the downtime during count mode. This is
// just used to increase subjective display brightness
// in modes where nothing needs to be done between segment
// updates
int scanDelay = 500;

// These variables are used to store the state of the IR
// interruptors between checks, particularly when counting
// in two-sensor up/down mode.
byte state_previous = 0b00;
byte state_a = 0b01;
byte state_b = 0b10;
bool bool_state_previous = 1;

// These bytes are used to store the state of every LED in the
// 7-segment display. They are essentially a frame buffer for the
// 7-seg.
byte digit1 = 0;
byte digit2 = 0;
byte digit3 = 0;

// Device Mode
// This is the state of the device wrt the top-level state machine.
// 0 - IDLE/STOP
// 1 - COUNTING
// 2 - RESET
// 3 - SETTINGS MENU
byte deviceMode = 0;

// Return Mode stores the state number of the mode that a given
// mode "exits" to. This is useful when going to mode 3 because
// we should always return from the Settings Menu into the mode
// that we entered from
byte returnMode;

// Counting Mode
// 0: Inventory Mode
// In Inventory Mode, we use only one IR interruptor. This is
// faster because we only need to read one ADC and the decision-making
// is simpler. Also, because it's direction agnostic, it correctly
// handles end/start of tape.
// 1: Dispense Mode
// In Dispense Mode we use both IR interruptors to create an encoder
// with the carrier tape. This allows us to determine the direction
// of the tape at the expense of counting more slowly. Also, because
// end/start of tape transitions look identical to motion in the
// opposite direction, we can't correctly count end/start of tape.
bool countingMode = 0;

// pitch of the parts on the carrier tape in mm
byte pitch = 4;

// ratio of holes in the carrier tape to actual parts,
// calculated based on the pitch
float ratio = 1.0;

// displayBuf is a place to store a value to be
// displayed on the 7-seg using the translateSegments()
// method
int displayBuf = 0;

// count is a place to store the parts count, especially
// when the display is showing something different
int count = 0;

// numPos is used to store the individual digits of a
// number in order of their decimal place. This is the
// output buffer of the numPositions() method. It's
// global to avoid passing memory pointers around and
// inevitably creating a hard to diagnose bug.
byte numPos[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// This is a lookup table of segment states in order
// their integer value. i.e. segLUT[6] returns the
// LED states to display a '6'
const byte segLUT[10] = {
    0b01110111,
    0b00100100,
    0b01011101,
    0b01011011,
    0b00111010,
    0b01101011,
    0b01101111,
    0b01010010,
    0b01111111,
    0b01111010};

// We don't do much in setup() just set some
// pinModes and initial pin states. We also check
// the EEPROM for saved user settings
void setup()
{

    for (int i = 0; i < 12; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, 0);
    }

    for (int i = 12; i < 14; i++)
    {
        pinMode(i, INPUT_PULLUP);
    }

    pinMode(IRSENSOR_A, INPUT);
    pinMode(IRSENSOR_B, INPUT);
    loadSettings();
    //digitalWrite(IRLED, 1);
}

// The main loop just manages the state machine
void loop()
{

    switch (deviceMode)
    {
    case 0:
        mode0();
        break;
    case 1:
        mode1();
        break;
    case 2:
        mode2();
        break;
    case 3:
        mode3();
        break;
    default:
        deviceMode = 0;
        break;
    }

  //ADC TEST MODE 
  //translateSegments(analogRead(A9));
  //updateLED();  
  
}

// Check EEPROM for user settings and load them
void loadSettings()
{
    byte nvmPitch, nvmMode;
    EEPROM.get(EEPROM_ADDR_PITCH, nvmPitch);
    EEPROM.get(EEPROM_ADDR_MODE, nvmMode);

    // We're hypervigilant about writing a valid value to pitch,
    // This way even if EEPROM gets corrupted by low battery,
    // At least the pitch ends up in a known and user-correctable
    // state
    if (nvmPitch == 1 || nvmPitch == 2 || nvmPitch == 4 || nvmPitch == 8 || nvmPitch == 12 || nvmPitch == 16)
    {
        pitch = nvmPitch;
        ratio = 4.0 / int(pitch);
    }

    if (nvmMode == 0x00)
    {
        countingMode = 0;
    }
    else if (nvmMode == 0x01)
    {
        countingMode = 1;
    }
    return;
}

// Mode 0: IDLE/STOP
// In this mode, the BeanCounter isn't counting. It just displays the last
// count and ignores the IR interruptors. To save battery, it also shuts off
// the IR illuminators.
void mode0()
{

    if (digitalRead(STARTBTN) == 0)
    {
        deviceMode = 2;
        returnMode = 1;
        return;
    }

    digitalWrite(IRLED, 0);

    byte blinkTimer = 0;

    while (deviceMode == 0)
    {

        blinkTimer++;

        updateLED();

        if (digitalRead(STARTBTN) == 0)
        {
            delay(500);
            deviceMode = 1;
        }

        if (digitalRead(SETBTN) == 0)
        {
            delay(500);
            deviceMode = 3;
            returnMode = 0;
        }

        if(blinkTimer > 125)
        {
            translateSegments(displayBuf);
        }
        else
        {
            digit1 = 0;
        }
    }
}

// Mode 1: COUNTING
// In this mode, we're just checking the user buttons between counting parts
// and updating the display. The actual counting (polling the IR interruptors)
// happens inside of updateLED() for timing reasons.
void mode1()
{

    if (digitalRead(STARTBTN) == 0)
    {
        deviceMode = 2;
        returnMode = 0;
        return;
    }

    digitalWrite(IRLED, 1);

    while (deviceMode == 1)
    {

        updateLED();

        if (digitalRead(STARTBTN) == 0)
        {
            delay(500);
            deviceMode = 0;
        }

        if (digitalRead(SETBTN) == 0)
        {
            delay(500);
            deviceMode = 3;
            returnMode = 1;
        }

        translateSegments(displayBuf);
    }
}

// Mode 2: RESET
// In this mode, we wait to see if the user really means it
// (if they're holding the button down) and then we reset the
// counter
void mode2()
{
    while (deviceMode == 2)
    {
        for (int i = 0; i < 100; i++)
        {
            updateLED();
        }
        delay(300);
        if (digitalRead(STARTBTN) == 1)
        {
            count = 0;
            displayBuf = 0;
            deviceMode = returnMode;
        }
    }
}

// Mode 3: SETTINGS MENU
// In mode 3, the SETUP button cycles through various settings that can be
// adjusted using the COUNT/RESET button. In order to exit the mode 3, the
// user must cycle through the entire settings menu. Probably OK because the
// user likely won't adjust the settings often.
void mode3()
{
    byte mode3state = 0;

    while (deviceMode == 3)
    {

        switch(mode3state)
        {
            case 0:
                if (digitalRead(STARTBTN) == 0)
                {
                    delay(200);
                    switch (pitch)
                    {
                    case 1:
                        pitch = 2;
                        break;
                    case 2:
                        pitch = 4;
                        break;
                    case 4:
                        pitch = 8;
                        break;
                    case 8:
                        pitch = 12;
                        break;
                    case 12:
                        pitch = 16;
                        break;
                    case 16:
                        pitch = 1;
                        break;
                    }
                    ratio = 4.0 / int(pitch);
                }
                if (digitalRead(SETBTN) == 0)
                {
                    delay(200);
                    mode3state = 1;
                }
                numPositions(pitch);
                digit3 = segLUT[numPos[0]];
                digit2 = segLUT[numPos[1]];
                digit1 = 0b01111100; // "P" for Pitch
                updateLED();
                break;
            
            case 1:
                if (digitalRead(STARTBTN) == 0)
                {
                    delay(200);
                    switch (countingMode)
                    {
                    case 0:
                        countingMode = 1;
                        count = 0;
                        displayBuf = 0;
                        break;
                    case 1:
                        countingMode = 0;
                        count = 0;
                        displayBuf = 0;
                        break;
                    }
                }
                if (digitalRead(SETBTN) == 0)
                {
                    delay(200);
                    mode3state = 2;
                }
                if(countingMode == 0){
                    digit1 = 0b00100100; // I
                    digit2 = 0b00001110; // n
                    digit3 = 0b00000111; // v
                }else{
                    digit1 = 0b00011111; // d
                    digit2 = 0b00100100; // I
                    digit3 = 0b01101011; // S
                }
                updateLED();
                break;

            case 2:
                // Check EEPROM to see if we've changed the settings
                // and if we have, save the new ones
                byte nvmPitch, nvmMode;
                EEPROM.get(EEPROM_ADDR_PITCH, nvmPitch);
                EEPROM.get(EEPROM_ADDR_MODE, nvmMode);
                if (nvmPitch != pitch)
                {
                    EEPROM.put(EEPROM_ADDR_PITCH, pitch);
                }
                if (nvmMode != countingMode)
                {
                    EEPROM.put(EEPROM_ADDR_MODE, countingMode);
                }                
                deviceMode = returnMode; // Return to whichever mode sent us here
                break;
        }

    }
}

// This method takes the int stored in displayBuf and
// shoves it into numPositions, which results in the int
// being split into digits. Each digit is fetched from the
// result array numPos[], translated into a String of LED
// segment states by segLUT[] and assigned to a
// display digit
void translateSegments(int displayBuf)
{
    numPositions(displayBuf);
    digit3 = segLUT[numPos[0]];
    digit2 = segLUT[numPos[1]];
    digit1 = segLUT[numPos[2]];
}

// This method is called to "scan" the 7-seg once. In order to
// achieve the Persistence of Vision effect necessary to make the
// display visible, this method needs to be called as often as possible.
// In order to keep BOM costs low, we aren't using any external comparators
// which means we need to use the ADCs on the uC. This also means that
// we can't update our counter in an interrupt. The only alternative
// is to poll the IR interruptors as often as possible. Luckily, we
// need to be busy periodically between updating each digit of the
// display, so we use that time to do our counting. If we don't need to
// be coounting, we busywait instead.
void updateLED()
{

    for (int i = 0; i < 7; i++)
    {
        digitalWrite(i, bitRead(digit1, 6-i));
    }

    digitalWrite(DIGIT1_CATHODE, 1);
    if (deviceMode == 1)
    {
        updateCount();
    }
    else
    {
        delayMicroseconds(scanDelay);
    }
    digitalWrite(DIGIT1_CATHODE, 0);

    for (int i = 0; i < 7; i++)
    {
        digitalWrite(i, bitRead(digit2, 6-i));
    }

    digitalWrite(DIGIT2_CATHODE, 1);
    if (deviceMode == 1)
    {
        updateCount();
    }
    else
    {
        delayMicroseconds(scanDelay);
    }
    digitalWrite(DIGIT2_CATHODE, 0);

    for (int i = 0; i < 7; i++)
    {
        digitalWrite(i, bitRead(digit3, 6-i));
    }

    digitalWrite(DIGIT3_CATHODE, 1);
    if (deviceMode == 1)
    {
        updateCount();
    }
    else
    {
        delayMicroseconds(scanDelay);
    }
    digitalWrite(DIGIT3_CATHODE, 0);
}

// This method takes an integer as an argument and returns
// an array of digits organized by decimal place.
void numPositions(int num)
{
    for (int i = 0; i < 10; i++)
    {
        numPos[i] = 0;
    }
    byte pos = 0;
    while (num != 0)
    {
        numPos[pos] = num % 10;
        num = num / 10;
        pos++;
    }
    return;
}

// This is the function that actually deals with the ADCs
// and turns blinking lights into parts counts. There are two
// modes we can use to count:
// Mode 0 is "Inventory Mode"
// In Inventory Mode, we use only one IR interruptor. This is
// faster because we only need to read one ADC and the decision-making
// is simpler. Also, because it's direction agnostic, it correctly
// handles end/start of tape.
// Mode 1 is "Dispense Mode"
// In Dispense Mode we use both IR interruptors to create an encoder
// with the carrier tape. This allows us to determine the direction
// of the tape at the expense of counting more slowly. Also, because
// end/start of tape transitions look identical to motion in the
// opposite direction, we can't correctly count end/start of tape.
void updateCount()
{

    if (countingMode == 1)
    {

        /**************** Dispense Mode ****************/
        // Fetch the ADC
        int adc_a = analogRead(IRSENSOR_A); 
        int adc_b = analogRead(IRSENSOR_B);

        // Here we create a deadzone between high and low 
        // sensor readings to prevent miscounting. Theoretically, 
        // you don't need to do this, but since this device operates 
        // in meatspace, it's possible for sensor values to fluctuate
        // even if the tape isn't moving. Without deadzone, it was 
        // possible to occlude the sensor just enough for it to start 
        // "counting" parts at 1/2 polling speed.
        // The state values may look like nonsense but it's really just 
        // assigning the state of each IR interruptor to a separate bit in 
        // a two bit binary number. This makes state transitions easier to 
        // track
        if (adc_a > 100)
        {
            state_a = 0b01;
        }
        else if (adc_a < 50)
        {
            state_a = 0b00;
        }
        if (adc_b > 100)
        {
            state_b = 0b10;
        }
        else if (adc_b < 50)
        {
            state_b = 0b00;
        }

        byte state_current = state_a + state_b;

        // If the state we just measured is different than the last time we were 
        // polled, it means the tape has moved! Time to count!
        if (state_current != state_previous)
        {
            // The state transitions for tape moving "up" are: 
            // 00 01 11 10
            // and for a tape moving "down" are: 
            // 00 10 11 01
            // but because of the geometry involved, the "00" 
            // state is very short, we shouldn't rely on detecting 
            // it. So we only increment/decrement on the transition 
            // out of "11" which is our longest state.
            if (state_previous == 0b11)
            {
                switch (state_current)
                {
                case 0b01:
                    count++;
                    // We don't keep track of which way we're counting
                    // so instead we just count into the negative and 
                    // display the absolute value
                    displayBuf = abs(count) * ratio; // multiply by hole-to-part ratio
                    break;
                case 0b10:
                    count--;
                    displayBuf = abs(count) * ratio;
                    break;
                }
            }
            state_previous = state_current;
        }

    }
    else
    {
        /**************** Inventory Mode ****************/
        // Fetch the ADC
        int adc_a = analogRead(IRSENSOR_A);

        // Create a bool to store the sensor state
        bool bool_state_current;

        // We need to do the deadzone trick here as well for stability
        if (adc_a > 100)
        {
            bool_state_current = 1;
        }
        else if (adc_a < 50)
        {
            bool_state_current = 0;
        }
        
        // If the state we just measured is different than the last time we were 
        // polled, it means the tape has moved! Time to count!
        if (bool_state_current != bool_state_previous)
        {
            if (bool_state_current == 1)
            {
                count++;
                displayBuf = count * ratio; // multiply by hole-to-part ratio
            }
            bool_state_previous = bool_state_current;
        }
    }
}
