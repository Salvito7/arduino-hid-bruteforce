


# Arduino HID Bruteforcer
This is a simple bruteforcer to crack a 4-digit combination from any mobile device with an OTG connection.

> ***If it wasn't obvious you need unlimited or at least 10^4 passcode
> tries on the target device for this to work.***


## Features
- OLED display support (u8g2 library)
- LED indicator 
- Pause button
- Known digits support 

## Requirements
 - Target device with OTG support
 - A HID-compatible microcontroller (Arduino Pro Micro, Arduino Leonardo, attiny85, ...)
 - VSCode with PlatformIO installed
 - Breadboard
 - Jumper wires
 - OLED display (list of compatible displays: https://github.com/olikraus/u8g2)
 - Photoresistor
 - 10kOhm resistor
 - LED (optional)
 - Button (optional)

**If you're using an iOS device you'll also need this:**
 - [Lightning to USB camera adapter](https://www.google.com/search?q=lightning+to+usb+camera+adapter&vssid=mosaic) (Make sure you get the one with the extra lightning port for power input,as some iOS devices can't power the board via OTG alone + you can charge your device while bruteforcing) 




> *I tested the sketch on the **Arduino Leonardo / Sparkfun Pro Micro**
> with a **SSD1306 128x64 OLED display**. 
> For any other board you have to change the board type and platform in
> the Platformio.ini file and maybe the button and OLED pins.*
  

## Step 1: Wiring and setup
With a **Sparkfun Pro Micro** a **SSD1306 128x64 OLED** display and **default pins** (table in step 2) the wiring should look something like this.

<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/fb69dd02-d872-4816-a93e-011ca08c75e2" width="70%">

(the photoresistor is missing in the picture below)

<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/dda26518-c769-4df3-bce4-f2c83b43f148" width="60%">

The photoresistor should ideally only receive light from the screen so you should cover it in black electrical tape and stick it to the phone screen. (Hint: see step 5 for checking the resistor value)

Example of how to isolate the photoresistor with two stripes of electrical tape.

<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/59d06670-4f8e-450b-904c-4e57c8729aff" width="30%">
<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/45005beb-7991-4373-84cf-87abb121769c" width="30%">

Pick a location for the photoresistor on the phone screen where the brightness doesn't change when the bruteforce is running.

**Finished setup:**

<img src="https://github.com/Salvito7/arduino-hid-bruteforce/assets/84077475/2f474c40-f286-42f3-9bbd-0f0c43c93225" width="60%">

(If you're using the USB camera adapter make sure you also plug in a power source)

## Step 2: Configure the sketch
There are a number of options to adapt the sketch to your needs.

|Var| Default | Description|
|--|--|--|
| `OLED_SCL` | 5 | OLED SDA-pin |
|`OLED_SDA` | 6 | OLED SDA-pin |
| `LED_PIN` | 17 | LED-pin, 17 = internal RX_LED of the Pro Micro |
|`BUTTON_PIN` | 2 | Button-pin can be used for enabling debugging and pausing the bruteforce, you can leave it to default if you don't use it |
|`RESISTOR_INPUT` | A0 | Photoresistor input-pin |
|`TOLERANCE` | 10 | Tolerance value for detecting a screen change. Default value should be fine in most cases. Higher = more light is needed to detect a screen change.  |
| `COOLDOWN` | 5500 | Delay in ms between bruteforce inputs |
| `s1 - s4`| 0 | If you already know a few digits you can type them in here as starting values. `s1` being the first digit to be typed|
|`debug` and `isPaused`| false | Go to step 5 for more info |


> *To change the display type please consult the link below and change
> the code accordingly: [supported displays](https://github.com/olikraus/u8g2/wiki/u8g2setupcpp)*

## Step 3: Upload to board

> *Don't forget to define the correct board in the platformio.ini
> file if you're using something other than the default 
> board (Leonardo / Pro Micro).*

## Step 4: Connect to the device & Start bruteforcing

> *Make sure that you plug in the board **while the device is on the passcode
> screen** otherwise it will calibrate incorrectly.*

If you setup everything correctly it should calibrate and start typing in pin numbers. 
Put the whole contraption somewhere safe and leave it running. 
Depending on the cooldown it should take **10 - 15 hours max.** to try all combinations 
(That's if you're really unlucky and the combination starts with a 9) 


## Step 5 (Optional): Debug
 - You can enable a debug option in the sketch by setting `bool debug` to `true` or by pressing the button in the calibration phase. 
 - If you only want to see if the photoresistor is setup correctly you should set both `debug` and `isPaused` to `true`, so that it pauses right at the beginning. 
 - The debug option displays the current light value that is received by the photoresistor and it helps to check if the photoresistor only receives light from the screen and no other external sources. 
 - The ideal screen-off value should be 0.

## Donating
If you want, you can support me here: 
- [Ko-Fi](https://ko-fi.com/salvito)
