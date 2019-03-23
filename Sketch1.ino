/*
 Name:		Base Hardware Code
 Created:	3/22/2019
 Author:	Spring Capstone 2019 - Mark, Vivetri, Babak, Robert, Yasir
*/

#include <LiquidCrystal.h>

// Proto-type function for adding delays in millisec

void time_delay_func(unsigned long int, const int);

// Setup for LCD Display

	const int rs = 12;
	const int en = 11;
	const int d4 = 5;
	const int d5 = 4;
	const int d6 = 3;
	const int d7 = 2;

	LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

	void lcd_display_func(int);
	void lcd_display_full(void);
	void lcd_display_empty(void);

//setup for Entry Push Button and Exit Button

	const int entry_button = 52;
	int button_state;
	int last_button_state = HIGH;
	unsigned long LastDebounceTime = 0;

	const int exit_button = 50;
	int button_state_exit;
	int last_button_state_exit = HIGH;
	unsigned long LastDebounceTimeExit = 0;

	unsigned long Debounced_Delay = 50;			// 50 msec

//debug LED
	const int debug_led = 53;
	int led_state = HIGH;

//global function for total number of parking
	unsigned int total_parking_spot = 6;

//the setup function runs once when you press reset or power the board
void setup() {

	//setting up LCD display
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("Park Total:6");

	//setting up entry button
	pinMode(entry_button, INPUT);
	pinMode(exit_button, INPUT);

	//debug led pin
	pinMode(debug_led, OUTPUT);
	Serial.begin(9600);

}

//the loop function runs over and over again until power down or reset
void loop() {


	int reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {

		LastDebounceTime = millis();

	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				led_state = !led_state;
			}

			// code for increment
			if ((button_state == LOW) && (total_parking_spot > 0)) {
				total_parking_spot--;
				lcd_display_func(total_parking_spot);
			}

			else if ((button_state == LOW) && (total_parking_spot == 0)) {
				lcd_display_full();
			}

		}
	}

	int reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {

		LastDebounceTimeExit = millis();

	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			// code for decrement
			if ((button_state_exit == LOW) && (total_parking_spot < 6) && (total_parking_spot >= 0)) {
				total_parking_spot++;
				lcd_display_func(total_parking_spot);
			}

			else if ((button_state_exit == LOW) && (total_parking_spot == 6)) {
				lcd_display_empty();
			}

		}
	}

	digitalWrite(debug_led, led_state);
	last_button_state = reading_entry_button;
	last_button_state_exit = reading_exit_button;

	Serial.print("Total Parking: ");
	Serial.println(total_parking_spot);
	
}

void time_delay_func(unsigned long int time_now, const int time_interval) {
	time_now = millis();
	while (millis() < time_now + time_interval) {
		// time delay in ms
	}
}

//lcd display user defined function

void lcd_display_func(int data) {
	lcd.setCursor(0, 1);
	lcd.print("Park Avail:");
	lcd.setCursor(11, 1);
	lcd.print(data);
}

void lcd_display_full(void) {
	lcd.setCursor(0, 1);
	lcd.print("Parking FULL!");
}

void lcd_display_empty(void) {
	lcd.setCursor(0, 1);
	lcd.print("     EMPTY!   ");
}

