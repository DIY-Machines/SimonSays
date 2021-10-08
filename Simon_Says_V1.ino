/*
Simon Says - DIY Machines

Print, assemble and program your own Simon Says game. How much of the randomly generated colour pattern can you memorise and repeat back? Not sure, then why not make one and find out. (My current best is level 14).

With an Arduino Nano at its core this project also incorporates an OLED screen, passive buzzer and some big bright buttons.

==========

List of items used in this project and where to find them (the links are Amazon affiliate links and help to support this channel at no cost to you):

■ Arduino Nano (x1): https://geni.us/ArduinoNanoV3
■ OLED Screen (1): https://geni.us/OLEDscreen
■ Battery Holders (x2): https://geni.us/2aaBatteryHolder
■ Resistors (220 Ohms) (x4) : https://geni.us/Ufa2s
■ Wire: https://geni.us/22AWGWire
■ AA Batteries (x4): https://geni.us/AABatteries
■ Illuminated Momentary Buttons (x4): https://geni.us/IllumMomentButtons
■ Nuts and Bolts - M3 x 10 (x4): http://geni.us/NutsAndBolts
■ 5v buzzer (x1) : https://geni.us/5vBuzzer
■ PLA Filament: https://geni.us/PLAFilament
■ Illuminated Power Switch (x1): https://geni.us/IllumPowerSwitch
■ Battery powered glue gun: http://geni.us/BoschBatteryGlueGun
■ Battery powered soldering iron: http://bit.ly/SealeySDL6

==========

The code and wiring diagram can be downloaded from here: http://www.diymachines.co.uk/diy-arduino-simon-says-game

3D printed parts can be downloaded from here:  http://www.diymachines.co.uk/diy-arduino-simon-says-game

==========

SAY THANKS:

Buy me a coffee to say thanks: https://ko-fi.com/diymachines
Support us on Patreon: https://www.patreon.com/diymachines

SUBSCRIBE:
■ https://www.youtube.com/channel/UC3jc4X-kEq-dEDYhQ8QoYnQ?sub_confirmation=1

INSTAGRAM: https://www.instagram.com/diy_machines/?hl=en
FACEBOOK: https://www.facebook.com/diymachines/

*/

#define led7 7             // white button led
#define led6 6             // green button led
#define led5 5             // red button led
#define led8 8             // blue button led

#define button7  led7 + 4  //white button
#define button6  led6 + 4  // green button
#define button5  led5 + 4  // red button
#define button8  led8 + 4  //blue button

#define levelsInGame 50
#define buzzer 3


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 32   // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4   // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//create an array for this game

int simonSaid[levelsInGame];                // initialise the array

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  // initialize digital pins as outputs for LEDS in the buttons.
  pinMode(led7, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led8, OUTPUT);

  digitalWrite(led7, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led8, LOW);

  pinMode(button7, INPUT_PULLUP);                     // set the button pins as inputs
  pinMode(button6, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  pinMode(button8, INPUT_PULLUP);

  randomSeed(analogRead(0));  // Make our random numbers more random

  // populate the array with random 'colours'
  for (int i = 0; i <= levelsInGame; i++) {
    simonSaid[i] = random(5, 9);
  }
  for(int j = 0; j < levelsInGame; j++)     //Print this array to serial monitor
  {
    Serial.println(simonSaid[j]);
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(1, 0);
  display.println("Simon Says");
  display.setTextSize(1); // Draw 1X-scale text
  display.println("   by DIY Machines");
  display.println("tinyurl.com/y2llr32t");
  display.display();      // Show text
  delay(2000);
}

// the loop function runs over and over again forever
void loop() {
  bool gameOver = 0;

  while (gameOver == 0) {
    for (int i = 1; i < (levelsInGame + 1); i++) {                  // for each level
      Serial.println();
      Serial.print("Level ");
      Serial.println(i);

      if (i < 10){
        display.clearDisplay();                                     // print level on screen
        display.setTextSize(3); // Draw 3X-scale text
        display.setTextColor(WHITE);
        display.setCursor(1, 0);
        display.print("Level ");
        display.println(i);
        display.display();      // Show text
        delay(400);
      } else {
        display.clearDisplay();                                     // print level on screen in smaller text
        display.setTextSize(2); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(1, 0);
        display.print(" Level ");
        display.println(i);
        display.display();      // Show text
        delay(400);
      }

      for (int g = 0; g < i; g++) {                                 // play the colours through to that level
        Serial.print("Colour number ");
        Serial.print(g + 1);
        Serial.print(" in this sequance is the button on D");
        Serial.println(simonSaid[g]);
        digitalWrite(simonSaid[g], HIGH);
        playBuzzer(simonSaid[g]);
        delay(200);
        digitalWrite(simonSaid[g], LOW);
        delay(50);
      }

      for (int g = 0; g < i; g++) {                                 // check the colours you enter for that level
        Serial.print("Press the correct button for number ");
        Serial.print(g + 1);
        Serial.print(" in Simon's sequance which should be the one connected to pin D");
        Serial.println(simonSaid[g]);
        int sensorButton7 = 1;
        int sensorButton6 = 1;
        int sensorButton5 = 1;
        int sensorButton8 = 1;

        int buttonPressed = 0;   //a button that does not exist
        while (sensorButton5 == HIGH && sensorButton6 == HIGH && sensorButton7 == HIGH && sensorButton8 == HIGH) { // until a button is pressed keep checking all of them
          sensorButton7 = digitalRead(button7);
          //Serial.print("Sensor button 0 = ");
          //Serial.println(sensorButton0);
          sensorButton6 = digitalRead(button6);
          //Serial.print("Sensor button 1 = ");
          //Serial.println(sensorButton1);
          sensorButton5 = digitalRead(button5);
          //Serial.print("Sensor button 2 = ");
          //Serial.println(sensorButton2);
          sensorButton8 = digitalRead(button8);
          //Serial.print("Sensor button 3 = ");
          // Serial.println(sensorButton3);
        }
        delay(200);      // a kind of debounce  - maybe later swap it for a check that all buttons have been depressed for 100ms?

        //record which one was pressed
        if (sensorButton7 == 0) {
          buttonPressed = 7;
        } else if (sensorButton6 == 0) {
          buttonPressed = 6;
        } else if (sensorButton5 == 0) {
          buttonPressed = 5;
        } else {
          buttonPressed = 8;
        }
        Serial.print("Button that was pressed is = ");         // print out the pressed button
        Serial.println(buttonPressed);
        digitalWrite(buttonPressed, HIGH);                     // turn on the button light
        playBuzzer(buttonPressed);                             // make the sound of the button pressed - right or wrong
        digitalWrite(buttonPressed, LOW);                      // turn off button light

        //check to see if it is what should be pressed
        if (buttonPressed == (simonSaid[g])) {
          Serial.println("Correct button - well done");
        } else {
          Serial.println("Wrong button was pressed.");
          gameOver = 1;
          Serial.println("Game finished - sorry. Better luck next time.");
          display.clearDisplay();
          display.setTextSize(2);     // Draw 2X-scale text
          display.setTextColor(WHITE);
          display.setCursor(1, 0);
          display.println("GAME OVER");
          display.print("at lvl ");
          display.print(i);
          display.display();          // Show initial text
          //display.startscrollleft(0x00, 0x0F);
          tone(buzzer, 350);          // play game over low tone on buzzer
          digitalWrite(led7, HIGH);   // turn on all the lights
          digitalWrite(led6, HIGH);
          digitalWrite(led5, HIGH);
          digitalWrite(led8, HIGH);
          delay(2000);
          noTone(buzzer);
          digitalWrite(led7, LOW);    //turn oFF all the lights
          digitalWrite(led6, LOW);
          digitalWrite(led5, LOW);
          digitalWrite(led8, LOW);
          delay(2000);
          softReset();
        }
      }
    }
  }
}

void softReset() {
  asm volatile ("  jmp 0");
}

void playBuzzer(int button) {
  if (button == 5) {
    tone(buzzer, 950);
    delay(300);
    noTone(buzzer);
  }
  else if (button == 6) {
    tone(buzzer, 850);
    delay(300);
    noTone(buzzer);
  }
  else if (button == 7) {
    tone(buzzer, 750);
    delay(300);
    noTone(buzzer);
  }
  else if (button == 8) {
    tone(buzzer, 650);
    delay(300);
    noTone(buzzer);
  }
}
