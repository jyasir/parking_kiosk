/*
 * One-time hardware and application initialization (runs once at boot).
 */
#include "parking_kiosk.h"

void kiosk_setup(void) {
	Serial.begin(9600);

	lcd_display_init();

	pinMode(entry_button, INPUT);
	pinMode(exit_button, INPUT);

	// setup for red led on six parking stations (digital 29-22, PORTA)
	DDRA = mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);
	/* breadboard pin location on red board
	   14 - 1A
	   13 - 1B
	   12 - 1C
	   10 - 2A
	   09 - 2B
	   08 - 2C */

	// setup for green led (digital 30-37, PORTC)
	DDRC = mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);
	/* breadboard pin location on red board
	   22 - 1A  - 37
	   21 - 1B  - 36
	   20 - 1C  - 35
	   18 - 2A  - 34
	   17 - 2B  - 33
	   16 - 2C  - 32 */

	PORTC |= mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);
	PORTA |= mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);

	pinMode(dip_switch_pin, INPUT);
	dip_switch = digitalRead(dip_switch_pin);

	if (dip_switch == LOW) {
		state = empty_a;
	}
	else if (dip_switch == HIGH) {
		state = empty_b;
	}
}
