#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

SoftwareSerial BLE(9, 10); // RX, TX kao version

String tmp="";

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
int isWipe = false;
int color[3];
char inChar;
char oldChar;
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

unsigned int color_val = 0;
int color_direction = 1;
int no = 0;

void setup() {
  BLE.begin(9600); 
  oldChar = 'x';
  
  Serial.begin(9600);
  
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  

}

void loop() {
  
  
  while (BLE.available() > 0)  {
    Serial.print("SERIAL AVAILABLE: ");
    Serial.println(BLE.available());    
    tmp += char(BLE.read());
    delay(2);
  }

  if(tmp.length() > 0) {
    
    Serial.print(tmp);
    Serial.print(" ");    
    
    inChar = tmp[0];

     if (tmp.indexOf("OK+ANCS801") == 0) {
       inChar = 'g';
     }
     else if (tmp.indexOf("OK+ANCS82") != -1) {
       inChar = 'r';
     }
     
    tmp = "";
    
    if (inChar == 'r') {
      color_val= 0;
      no = 0;      
      color_direction = 1;      
      fadeRed(0);
      oldChar = inChar;
    } else if (inChar == 'g') {
      color_val= 0;
      no = 0;      
      color_direction = 1;      
      fadeGreen(0);
      oldChar = inChar;
    }
  }
  else {
      fade(oldChar, no, color_val, color_direction);
      delay(5);
  }
}


void fade(char c, int n, unsigned int color, int d) {
//  Serial.print(c);
//  Serial.print(" ");
//  
//  Serial.print(n);
//  Serial.print(" ");
//
//  Serial.print(color);
//  Serial.print(" ");
//  
//  Serial.println();
//  
  
  if (c == 'r') {
    strip.setPixelColor(n, strip.Color(color, 0, 0));
    strip.show();
    
    color_val+= d;
    
    if (color >=  254) {
      color_direction = -1;
    }
    
    if (color<=2) {
      color_direction = 1;
    }
  }
  else if (c == 'g') {
    strip.setPixelColor(n, strip.Color(0, color, 0));
    strip.show();
    
    color_val+= d;
    
    if (color >=  254) {
      color_direction = -1;
    }
    
    if (color<=2) {
      color_direction = 1;
    }
  }  

}

// Fill the dots one after the other with a color
void fadeRed(uint8_t wait) {
  Serial.println("FADING RED");
  for (int ff = 0; ff < 255; ff+=50) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(ff, 0, 0));
        strip.show();
        delay(wait);
    }
  }
  
  for (int gg = 255; gg > 0; gg-=50) {
    for(uint16_t j=0; j<strip.numPixels(); j++) {
        if (gg <= 50) gg = 0;      
        strip.setPixelColor(j, strip.Color(gg, 0, 0));
        strip.show();
        delay(wait);
    }
  }
}

// Fill the dots one after the other with a color
void fadeGreen(uint8_t wait) {
  Serial.println("FADING GREEN");
  for (int ff = 0; ff < 255; ff+=50) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, ff, 0));
        strip.show();
        delay(wait);
    }
  }
  
  for (int gg = 255; gg > 0; gg-=50) {
    for(uint16_t j=0; j<strip.numPixels(); j++) {
        if (gg <= 50) gg = 0;      
        strip.setPixelColor(j, strip.Color(0, gg, 0));
        strip.show();
        delay(wait);
    }
  }
}

