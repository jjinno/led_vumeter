#include <stdarg.h>

#include "SPI.h"

#define __WS2801

#ifdef __WS2801
#include "Adafruit_WS2801.h"
#else
#include <Adafruit_NeoPixel.h>
#endif

#define ENVELOPE          A4
#define GATE              A5
#define LED_STRIP_LENGTH  54   // The total number of LED pixels
#define DATA_PIN          2    // Data wire for pixels (both WS2801 & WS2811 style)

#define SAMPLE_COUNT_MAX  10
#define VU_HEIGHT         9

#ifdef __WS2801
#define CLOCK_PIN         3    // Clock wire for WS2801-style pixels only
Adafruit_WS2801 strip = Adafruit_WS2801(LED_STRIP_LENGTH, DATA_PIN, CLOCK_PIN);
#else
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_STRIP_LENGTH, DATA_PIN, NEO_GRB + NEO_KHZ800);
#endif

#define RED               Color(255,0,0)
#define YELLOW            Color(255,255,0)
#define GREEN             Color(0,255,0)
#define OFF               0

int vol=0;
int avg=0;
int minVol=1024;
int maxVol=0;
int avgVol=0;

int sampleCount = 0;

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = b;
  c <<= 8;
  c |= r;
  c <<= 8;
  c |= g;
  return c;
}

void displayVolume(int vol, int zones)
{
  int rows = 0;
  int range = maxVol - minVol;
  int zoneSize = (range / zones) + 1;
  
  rows =  vol / zoneSize;
  switch (rows) {
    case 9:
      strip.setPixelColor(51, RED);
      strip.setPixelColor(50, RED);
      strip.setPixelColor(45, RED);
      strip.setPixelColor(44, RED);
    case 8:
      strip.setPixelColor(52, RED);
      strip.setPixelColor(49, RED);
      strip.setPixelColor(46, RED);
      strip.setPixelColor(43, RED);
    case 7:
      strip.setPixelColor(53, YELLOW);
      strip.setPixelColor(48, YELLOW);
      strip.setPixelColor(47, YELLOW);
      strip.setPixelColor(42, YELLOW);
    case 6:
      strip.setPixelColor(5, YELLOW);
      strip.setPixelColor(6, YELLOW);
      strip.setPixelColor(17, YELLOW);
      strip.setPixelColor(18, YELLOW);
      strip.setPixelColor(29, YELLOW);
      strip.setPixelColor(30, YELLOW);
      strip.setPixelColor(41, YELLOW);
    case 5:
      strip.setPixelColor(4, YELLOW);
      strip.setPixelColor(7, YELLOW);
      strip.setPixelColor(16, YELLOW);
      strip.setPixelColor(19, YELLOW);
      strip.setPixelColor(28, YELLOW);
      strip.setPixelColor(31, YELLOW);
      strip.setPixelColor(40, YELLOW);
    case 4:
      strip.setPixelColor(3, YELLOW);
      strip.setPixelColor(8, YELLOW);
      strip.setPixelColor(15, YELLOW);
      strip.setPixelColor(20, YELLOW);
      strip.setPixelColor(27, YELLOW);
      strip.setPixelColor(32, YELLOW);
      strip.setPixelColor(39, YELLOW);
    case 3:
      strip.setPixelColor(2, GREEN);
      strip.setPixelColor(9, GREEN);
      strip.setPixelColor(14, GREEN);
      strip.setPixelColor(21, GREEN);
      strip.setPixelColor(26, GREEN);
      strip.setPixelColor(33, GREEN);
      strip.setPixelColor(38, GREEN);
    case 2:
      strip.setPixelColor(1, GREEN);
      strip.setPixelColor(10, GREEN);
      strip.setPixelColor(13, GREEN);
      strip.setPixelColor(22, GREEN);
      strip.setPixelColor(25, GREEN);
      strip.setPixelColor(34, GREEN);
      strip.setPixelColor(37, GREEN);
    case 1:
      strip.setPixelColor(0, GREEN);
      strip.setPixelColor(11, GREEN);
      strip.setPixelColor(12, GREEN);
      strip.setPixelColor(23, GREEN);
      strip.setPixelColor(24, GREEN);
      strip.setPixelColor(35, GREEN);
      strip.setPixelColor(36, GREEN);
      break;
  }
  
  strip.show();
  delay(10);
}

void clearVolume()
{
  for (int j=0; j<LED_STRIP_LENGTH; j++)
  {
    strip.setPixelColor(j, OFF);
  }
  strip.show();
}

void setup() 
{
  // put your setup code here, to run once:
  pinMode(ENVELOPE, INPUT);
  pinMode(GATE, INPUT);
  
  
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() 
{
  vol = analogRead(ENVELOPE);
  
  // Compute an average
  avg = ((sampleCount * avgVol) + vol) / (sampleCount + 1);
  if (sampleCount < SAMPLE_COUNT_MAX) sampleCount++;
  avgVol = avg;
  
  // Set maxVol using simple max...
  if (vol > maxVol) maxVol = vol;

  // minVol is a % of maxVol
  minVol = (avgVol / (VU_HEIGHT + 1));

  // put your main code here, to run repeatedly:
  displayVolume(vol, VU_HEIGHT);
  clearVolume();
}
