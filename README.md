

# Arduino HID Bruteforcer
This is a simple bruteforcer to crack a 4-digit combination from any mobile device with an OTG connection.

> If it wasn't obvious you need  unlimited- or at least 10^4 passcode
> tries on the target device for this to work.

## Requirements

 - A HID-compatible microcontroller (Arduino Pro Micro, Arduino Leonardo, attiny85, ...)
 - Breadboard
 - Jumper wires
 - OLED display (list of compatible displays: https://github.com/olikraus/u8g2)
 - Photoresistor
 - 10kOhm resistor
 - Lightning to USB camera adapter (If you're using an iOS device) 
 - LED (optional)
 - Button (optional)

> *I tested the sketch only on the **Arduino Leonardo / Sparkfun Pro Micro**
> with a **SSD1306 128x64 OLED display**. 
> For any other board you have to change the board type and platform in
> the PlatformIO settings and maybe the button and OLED pins.*
  

## Step 1: Wiring and setup
With a **Sparkfun Pro Micro** a **SSD1306 128x64 OLED** display and **default sketch pins** the wiring should look something like this.

<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/fb69dd02-d872-4816-a93e-011ca08c75e2" width="70%">

(the photoresistor is missing in the picture below)

<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/dda26518-c769-4df3-bce4-f2c83b43f148" width="60%">

The photoresistor should ideally only receive light from the screen so you should cover it in black electrical tape and stick it to the phone screen.
Example of how to isolate the photoresistor with two stripes of electrical tape.

<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/59d06670-4f8e-450b-904c-4e57c8729aff" width="30%">
<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/45005beb-7991-4373-84cf-87abb121769c" width="30%">

Pick a location on the phone screen where the brightness doesn't change when the bruteforce is running.

Finished setup:

<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/2f474c40-f286-42f3-9bbd-0f0c43c93225" width="60%">


## Step 2: Configure the sketch
There are a number of options to adapt the sketch to your needs.
 - `OLED_SCL` - OLED SCL-pin 
 - `OLED_SDA` - OLED SDA-pin 
 - `LED_PIN` -  LED-pin (default: 17 = internal RX_LED of the pro micro)
 - `BUTTON_PIN` - Button-pin (used for enabling debugging and pausing the bruteforce)
 - `RESISTOR_INPUT` -  Photoresistor input-pin
 - `TOLERANCE` - Tolerance value for detecting a screen change (don't set it too low)
 - `COOLDOWN` - Delay in ms between bruteforce inputs

> *To change the display type please consult the list below and change
> the code accordingly: [supported displays](https://github.com/olikraus/u8g2/wiki/u8g2setupcpp)*

## Step 3: Upload to board

> *Don't forget to define the correct board in the platformio.ini
> file if you're using something other than the default 
> board (Leonardo / Pro Micro).*

## Step 4: Connect to the device & Start bruteforcing
If you setup everything correctly it should calibrate successfully and start typing pin numbers. 
Put the whole contraption somewhere safe and leave it running 
Depending on the cooldown it should take about **1-2 days** to try all combinations. 
> *Make sure that you plug in the board **while you're on the passcode
> screen** of the device otherwise it will calibrate incorrectly.*

## Step 5 (Optional): Debug
 You can enable a debug option in the sketch by setting `bool debug` to `true` or by pressing the button in the calibration phase.
 The Debug option displays the current light value that is received by the photoresistor and it helps to check if  the photoresistor only receives light from the screen and no other external sources. 
 The screen-off value should be 0.
