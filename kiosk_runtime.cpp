/*
 * Main loop and parking state machine (runs continuously).
 */
#include "parking_kiosk.h"

void kiosk_loop(void) {
	state();

	Serial.print("State: ");
	Serial.println(debug_state);
}

void time_delay_func(unsigned long time_now, const uint16_t time_interval) {
	time_now = millis();
	while (millis() < time_now + time_interval) {
	}
}

void lcd_display_func(int data) {
	lcd.clear();
	lcd.setCursor(0, 1);
	lcd.print("Park Avail:");
	lcd.setCursor(11, 1);
	lcd.print(data);
}

void lcd_display_full(void) {
	lcd.clear();
	lcd.setCursor(0, 1);
	lcd.print("Parking FULL!");
}

void lcd_display_empty(void) {
	lcd.clear();
	lcd.setCursor(0, 1);
	lcd.print("     EMPTY!   ");
}

void lcd_display_position(char* pos) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Park at position: ");
	lcd.setCursor(0, 1);
	lcd.print(pos);
}

void lcd_display_init(void) {
	lcd.clear();
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("Park Avail:");
	lcd.setCursor(11, 0);
	lcd.print(MAX);
}

void green_led_on(const uint8_t num) {
	PORTC &= ~mask(num);
}
void green_led_off(const uint8_t num) {
	PORTC |= mask(num);
}
void red_led_on(const uint8_t num) {
	PORTA &= ~mask(num);
}
void red_led_off(const uint8_t num) {
	PORTA |= mask(num);
}

void exit_button_func(void) {
	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();

	}
	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				if (dip_switch == LOW) {
					state = empty_a;
				}
				else if (dip_switch == HIGH) {
					state = empty_b;
				}
				if (total_parking > 0) {
					total_parking = total_parking - 1;
				}
			}
		}
	}

	last_button_state_exit = reading_exit_button;
}

void empty_a(void) {
	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}
	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[0].ping_cm() > 10) {
					total_parking = total_parking + 1;
					lcd_display_position("1A");
					state = car_entrance1_a;
				}
				else {
					state = car_entrance2_a;
				}
			}

		}

	}

	last_button_state = reading_entry_button;

	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();
	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				state = empty_a;
				if (total_parking > 0) {
					total_parking--;
				}
				if (total_parking == 0) {
					lcd_display_empty();
				}
			}
		}
	}

	last_button_state_exit = reading_exit_button;

	for (uint8_t i = 0; i < MAX; i++) {
		if (Sensor[i].ping_cm() > 10) {
			green_led_off(i);
			red_led_off(i);
		}
	}

	Serial.println(Sensor[0].ping_cm());
	debug_state = 0;
}

void car_entrance1_a(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[1].ping_cm() > 10) {
					total_parking = total_parking + 1;
					lcd_display_position("1B");
					state = car_entrance2_a;
				}
				else {
					state = car_entrance3_a;
				}
			}

		}

	}

	if (Sensor[0].ping_cm() < 10) {
		green_led_off(0);
		red_led_on(0);
	}
	else {
		green_led_on(0);
		red_led_off(0);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	Serial.println(Sensor[1].ping_cm());
	debug_state = 1;
}

void car_entrance2_a(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[2].ping_cm() > 10) {

					total_parking = total_parking + 1;
					lcd_display_position("1C");
					state = car_entrance3_a;
				}
				else {
					state = car_entrance4_a;
				}
			}

		}

	}

	if (Sensor[1].ping_cm() < 10) {
		green_led_off(1);
		red_led_on(1);
	}
	else {
		green_led_on(1);
		red_led_off(1);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	Serial.println(Sensor[2].ping_cm());
	debug_state = 2;
}

void car_entrance3_a(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[3].ping_cm() > 10) {

					total_parking = total_parking + 1;
					lcd_display_position("2A");
					state = car_entrance4_a;
				}
				else {
					state = car_entrance5_a;
				}
			}

		}

	}

	if (Sensor[2].ping_cm() < 10) {
		green_led_off(2);
		red_led_on(2);
	}
	else {
		green_led_on(2);
		red_led_off(2);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 3;
}

void car_entrance4_a(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[4].ping_cm() > 10) {

					total_parking = total_parking + 1;
					lcd_display_position("2B");
					state = car_entrance5_a;
				}
				else {
					state = car_entrance6_a;
				}
			}

		}

	}

	if (Sensor[3].ping_cm() < 10) {
		green_led_off(3);
		red_led_on(3);
	}
	else {
		green_led_on(3);
		red_led_off(3);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 4;
	Serial.print("Sensor Station 2B: ");
	Serial.println(Sensor[4].ping_cm());
}

void car_entrance5_a(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[5].ping_cm() > 10) {

					total_parking = total_parking + 1;
					lcd_display_position("2C");
					state = car_entrance6_a;
				}
				else {
					state = full;
				}
			}

		}

	}

	if (Sensor[4].ping_cm() < 10) {
		green_led_off(4);
		red_led_on(4);
	}
	else {
		green_led_on(4);
		red_led_off(4);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 5;
}

void car_entrance6_a(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				lcd_display_full();
				state = full;

			}

		}

	}

	if (Sensor[5].ping_cm() < 10) {
		green_led_off(5);
		red_led_on(5);
	}
	else {
		green_led_on(5);
		red_led_off(5);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 6;
}

void full(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				state = full;
				lcd_display_full();
			}

		}

	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 7;
}

void empty_b(void) {
	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}
	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[3].ping_cm() > 10) {
					lcd_display_position("2A");
					state = car_entrance1_b;
					total_parking = total_parking + 1;

				}
				else {
					state = car_entrance2_b;
				}
			}

		}

	}

	last_button_state = reading_entry_button;

	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();
	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				state = empty_b;
				lcd_display_empty();
				if (total_parking > 0) {
					total_parking--;
				}
			}
		}
	}

	last_button_state_exit = reading_exit_button;

	for (uint8_t i = 0; i < MAX; i++) {
		if (Sensor[i].ping_cm() > 10) {
			green_led_off(i);
			red_led_off(i);
		}
	}

	debug_state = 10;
}

void car_entrance1_b(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[4].ping_cm() > 10) {
					total_parking = total_parking + 1;
					lcd_display_position("2B");
					state = car_entrance2_b;
				}
				else {
					state = car_entrance3_b;
				}
			}

		}

	}

	if (Sensor[3].ping_cm() < 10) {
		green_led_off(3);
		red_led_on(3);
	}
	else {
		green_led_on(3);
		red_led_off(3);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 11;
}

void car_entrance2_b(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[5].ping_cm() > 10) {

					total_parking = total_parking + 1;
					lcd_display_position("2C");
					state = car_entrance3_b;
					green_led_on(5);
				}
				else {
					state = car_entrance4_b;
				}
			}

		}

	}

	if (Sensor[4].ping_cm() < 10) {
		green_led_off(4);
		red_led_on(4);
	}
	else {
		green_led_on(4);
		red_led_off(4);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 12;
}

void car_entrance3_b(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[0].ping_cm() > 10) {
					total_parking = total_parking + 1;
					lcd_display_position("1A");
					state = car_entrance4_b;
				}
				else {
					state = car_entrance5_b;
				}
			}

		}

	}

	if (Sensor[5].ping_cm() < 10) {
		green_led_off(5);
		red_led_on(5);
	}
	else {
		green_led_on(5);
		red_led_off(5);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 13;
}

void car_entrance4_b(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[1].ping_cm() > 10) {
					total_parking = total_parking + 1;
					lcd_display_position("1B");
					state = car_entrance5_b;
				}
				else {
					state = car_entrance6_b;
				}
			}

		}

	}

	if (Sensor[0].ping_cm() < 10) {
		green_led_off(0);
		red_led_on(0);
	}
	else {
		green_led_on(0);
		red_led_off(0);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 14;
}

void car_entrance5_b(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[2].ping_cm() > 10) {
					total_parking = total_parking + 1;
					lcd_display_position("1C");
					state = car_entrance6_b;
				}
				else {
					state = full;
				}
			}

		}

	}

	if (Sensor[1].ping_cm() < 10) {
		green_led_off(1);
		red_led_on(1);
	}
	else {
		green_led_on(1);
		red_led_off(1);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 15;
}

void car_entrance6_b(void) {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				lcd_display_full();
				state = full;
			}

		}

	}

	if (Sensor[2].ping_cm() < 10) {
		green_led_off(2);
		red_led_on(2);
	}
	else {
		green_led_on(2);
		red_led_off(2);
	}

	last_button_state = reading_entry_button;

	exit_button_func();

	debug_state = 16;
}
