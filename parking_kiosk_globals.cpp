/*
 * Global hardware objects and mutable runtime state.
 */
#include "parking_kiosk.h"

const uint8_t rs = 12;
const uint8_t en = 11;
const uint8_t d4 = 5;
const uint8_t d5 = 4;
const uint8_t d6 = 3;
const uint8_t d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const uint8_t entry_button = 52;
short button_state;
int last_button_state = HIGH;
unsigned long LastDebounceTime = 0;

const uint8_t exit_button = 50;
short button_state_exit;
int last_button_state_exit = HIGH;
unsigned long LastDebounceTimeExit = 0;

const unsigned long Debounced_Delay = 50;

unsigned short total_parking_spot = MAX;
uint8_t total_parking = 0;

const uint8_t trig1 = 8;
const uint8_t echo1 = 9;
const uint8_t trig2 = 6;
const uint8_t echo2 = 7;
const uint8_t trig3 = 13;
const uint8_t echo3 = 10;
const uint8_t trig4 = 48;
const uint8_t echo4 = 49;
const uint8_t trig5 = 46;
const uint8_t echo5 = 47;
const uint8_t trig6 = 44;
const uint8_t echo6 = 45;

NewPing Sensor[MAX] = {
	NewPing(trig1, echo1, 500),
	NewPing(trig2, echo2, 500),
	NewPing(trig3, echo3, 500),
	NewPing(trig4, echo4, 500),
	NewPing(trig5, echo5, 500),
	NewPing(trig6, echo6, 500)
};

const int dip_switch_pin = 40;
int dip_switch;

uint8_t debug_state;

void (*state)();
