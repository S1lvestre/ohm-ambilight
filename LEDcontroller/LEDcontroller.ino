/* ***** ***** ***** ***** ***** ***** */
/* FastLED vars                        */
/* ***** ***** ***** ***** ***** ***** */

#include <FastLED.h>

#define LED_TYPE APA102
#define COLOR_ORDER RGB
#define NUM_LEDS 78

#define DATA_PIN 2
#define CLOCK_PIN 3

CRGB strip[NUM_LEDS];

/* ***** ***** ***** ***** ***** ***** */
/* Serial vars                         */
/* ***** ***** ***** ***** ***** ***** */

const byte buffSize = 15;

const byte startMarker = '<';
const byte endMarker = '>';
const byte nextHexMarker = ';';

char buffer[buffSize];
byte rcvd;

long colors[NUM_LEDS];
byte led;

char goingOut[8];

boolean receiving = false;
boolean mayApplyColors = false;

// ===============
//
//      SETUP
//
// ===============

void setup()
{
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(strip, NUM_LEDS);
    FastLED.setBrightness(75);

    Serial.begin(500000); //187800
    Serial.println("<Arduino is Ready>");
}

// ===============
//
//      LOOP
//
// ===============

void loop()
{
    getSerialData();
    
    applyColors();
}

// ===============
//
//    FUNCTIONS
//
// ===============


void getSerialData()
{
    if( Serial.available() > 0 )
    {
        byte x = Serial.read();
        
        if( x == startMarker )
        {     
            rcvd = 0;
            led = 0;
            receiving = true;
        }
        else if( receiving )
        {
            if( x == nextHexMarker )
            {
                colors[led] = strtol(buffer, NULL, 16);
                
                led ++;
                rcvd = 0;
            }
            else if( x == endMarker )
            {
                receiving = false;
                mayApplyColors = true;
            }
            else
            {
                buffer[rcvd] = x;
                rcvd ++;
            }
        }
    }
}

void applyColors()
{
    if( mayApplyColors )
    {
        led = 0;
        while( led < NUM_LEDS )
        {
            strip[led] = colors[led];
            led++;
        }
        FastLED.show();
        
        mayApplyColors = false;
        
/*      Serial.println("<Send Next>"); */
    }
}

