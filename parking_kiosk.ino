/*
 code: hardware pass 4 revision
 date: 04/15/2019
 developer: Yasir Jamal
 uchl id: 1141556
 project: Spring Captsone - Smart Parking Kiosk

 Modular layout: see parking_kiosk.h, kiosk_setup.cpp, kiosk_runtime.cpp, parking_kiosk_globals.cpp
*/

#include "parking_kiosk.h"

void setup() {
	kiosk_setup();
}

void loop() {
	kiosk_loop();
}

// Bug Fixes
// 1. update the display feature at each state after the car leaves.
