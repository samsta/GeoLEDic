#include "Arduino.h"
#include <OctoWS2811.h>
#define FASTLED_INTERNAL // get rid of annoying version pragma
#include "FastLED.h"
#include "GeoLEDic.hpp"
#include "Controls.hpp"

#define FRAME_RATE 24
static const unsigned FRAME_INTERVAL_MS = 1000 / FRAME_RATE;

static const int LOADMON_PIN = 34;

DMAMEM int dma_mem[LEDS_PER_STRIP * 3 * (NUM_STRIPS + 3)/4];

static const uint8_t pinlist[NUM_STRIPS] = {
/*  S0 */  0, 
/*  S1 */  4, 
/*  S2 */  6,
/*  S3 */  7, 
/*  S4 */  8, 
/*  S5 */  9, 
/*  S6 */ 10,
/*  S7 */ 12,
/*  S8 */ 14, 
/*  S9 */  1, 
/* S10 */  5, 
/* S11 */ 11,
/* S12 */ 15, 
/* S13 */  2, 
/* S14 */  3, 
/* S15 */ 21,
/* S16 */ 17, 
/* S17 */ 18, 
/* S18 */ 19, 
/* S19 */ 30,
/* S20 */ 16, 
/* S21 */ 31,
/* S22 */ 28,
/* S23 */ 22, 
/* S24 */ 20, 
/* S25 */ 29,
/* S26 */ 25,
/* S27 */ 26,
/* S28 */ 27,
/* S29 */ 23, 
/* S30 */ 24, 
/* S31 */ 32
};

OctoWS2811 octo_ws(LEDS_PER_STRIP, dma_mem, leds, WS2811_GRB | WS2813_800kHz, NUM_STRIPS, pinlist);

void setup() {
#ifdef USB_MIDI_SERIAL    
    Serial.begin(115200); 
#endif   
    delay(1000); // sanity delay
    pinMode(LOADMON_PIN, OUTPUT);

    octo_ws.begin();
    octo_ws.registerForceBlank(Controls::getForceBlank());
}

void loop()
{
    uint32_t time_before = millis();
    digitalWrite(LOADMON_PIN, HIGH);
    loopGeoLEDic();
    octo_ws.show();

    uint32_t dt = millis() - time_before;
    digitalWrite(LOADMON_PIN, LOW);
    delay(FRAME_INTERVAL_MS - dt);
}
