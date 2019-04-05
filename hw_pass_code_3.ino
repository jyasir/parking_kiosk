/*
 Name:		Third Hardware Pass Code
 Created:	04/04/2019
 Author:	Spring Capstone 2019 - Mark, Vivetri, Babak, Robert, Yasir
*/

#include <stdint.h>
#include <LiquidCrystal.h>
#include <NewPing.h>
	
#define mask(x) ((uint8_t) (1 << x))
#define MAX 3
#define MIN 0

//function proto

void time_delay_func(unsigned long, const uint16_t);
void lcd_display_init();
void lcd_display_func(int);
void lcd_display_full(void);
void lcd_display_empty(void);
void lcd_display_position(char*);
void green_led_on(const uint8_t);
void green_led_off(const uint8_t);
void red_led_on(const uint8_t);
void red_led_off(const uint8_t);

void (*state)();
void empty();
void car_entrance1();
void car_entrance2();
void car_entrance3();
void full();

// Setup for LCD Display
const uint8_t rs = 12;
const uint8_t en = 11;
const uint8_t d4 = 5;
const uint8_t d5 = 4;
const uint8_t d6 = 3;
const uint8_t d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint8_t stat = 0;


//setup for Entry Push Button and Exit Button
const uint8_t entry_button = 52;
short button_state;
int last_button_state = HIGH;
unsigned long LastDebounceTime = 0;
const uint8_t exit_button = 50;
short button_state_exit;
int last_button_state_exit = HIGH;
unsigned long LastDebounceTimeExit = 0;
const unsigned long Debounced_Delay = 50;			// 50 msec

//global function for total number of parking
unsigned short total_parking_spot = MAX;
uint8_t total_parking = 0;

//parking matrix integer
uint8_t parking_spot[MAX] = {};

//setting ultra-motion sensor pins

const uint8_t trig1 = 8;
const uint8_t echo1 = 9;
const uint8_t trig2 = 6;
const uint8_t echo2 = 7;
const uint8_t trig3 = 13;
const uint8_t echo3 = 10;

NewPing Sensor[MAX] = {
	NewPing(trig1, echo1, 500),
	NewPing(trig2, echo2, 500),
	NewPing(trig3, echo3, 500)
};

//the setup function runs once when you press reset or power the board
void setup() {

	//opening up the serial port
	Serial.begin(9600);

	//setting up LCD display
	lcd_display_init();

	//setting up entry button
	pinMode(entry_button, INPUT);
	pinMode(exit_button, INPUT);

	//setup for red led on six parking stations
	//digital signals 29-22 [MSB - LSB]
	//PORTA 
	DDRA = mask(7) | mask(6) | mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);

	//setup for green led on six parking stations
	//digital signals 30-37 [MSB - LSB]
	//PORTC
	DDRC = mask(7) | mask(6) | mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);


	// Set the GREEN and RED LED OFF
	PORTA |= mask(2) | mask(1) | mask(0);
	PORTC |= mask(2) | mask(1) | mask(0);

	state = empty;
}

//the loop function runs over and over again until power down or reset
void loop() {

	state();

	//debug info

	Serial.print("Cars in Parking: ");
	Serial.println(total_parking);
	Serial.print("State: ");
	Serial.println(stat);

}

//time delay user defined function in millisec()

void time_delay_func(unsigned long time_now, const uint16_t time_interval) {
	time_now = millis();
	while (millis() < time_now + time_interval) {
		// time delay in ms
	}
}

//lcd display user defined function

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

void lcd_display_init() {
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


// state functions

void empty() {

	// car entrance section - starts

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW)  { 
				if (Sensor[2].ping_cm() > 10) {
					state = car_entrance1;
					total_parking = total_parking + 1;
					lcd_display_position("1A");
					green_led_on(0);
				}
				else {
					state = car_entrance2;
				}
			}

		}

	}

	last_button_state = reading_entry_button;

	// car entrance section - ends

	// car exit section - starts

	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();
	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				state = empty;
				lcd_display_empty();
				if (total_parking > 0) {
					total_parking--;
				}
			}
		}
	}


	last_button_state_exit = reading_exit_button;

	// car exit section - ends

	// resetting sensors

	for (uint8_t i = 0; i < MAX; i++) {
		if (Sensor[i].ping_cm() > 10) {
			green_led_off(i);
			red_led_off(i);
		}
	}

	// Debug Info

	stat = 1;

}

void car_entrance1() {

	// car entrance - starts

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[1].ping_cm() > 10) {
					state = car_entrance2;
					total_parking = total_parking + 1;
					lcd_display_position("1B");
					green_led_on(1);
				}
				else {
					state = car_entrance3;
				}
			}

		}

	}

	if (Sensor[2].ping_cm() < 4) {
		green_led_off(0);
		red_led_on(2);
	}
	else {
		green_led_on(0);
		red_led_off(2);
	}

	last_button_state = reading_entry_button;

	// car entrance - ends

	// car exit - starts

	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();

	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				state = empty;
				total_parking = total_parking - 1;
			}
		}
	}

	last_button_state_exit = reading_exit_button;

	// car exit - ends

	// Debug Info

	Serial.print("Sensor Reading: ");
	Serial.println(Sensor[2].ping_cm());
	Serial.print("Green LED Status: ");
	Serial.println(~(PORTC) & mask(0));
	Serial.print("Red LED Status: ");
	Serial.println(~(PORTA) & mask(2));

	stat = 2;

}

void car_entrance2() {

	// car entrance - starts

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				if (Sensor[0].ping_cm() > 10) {
					state = car_entrance3;
					total_parking = total_parking + 1;
					lcd_display_position("1C");
				}
				else {
					state = full;
				}
			}

		}

	}
	if (Sensor[1].ping_cm() < 4) {
		green_led_off(1);
		red_led_on(1);
	}
	else {
		green_led_on(1);
		red_led_off(1);
	}

	last_button_state = reading_entry_button;

	// car entrance - ends

	// car exit - starts

	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();

	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				state = empty;
				total_parking = total_parking - 1;
			}
		}
	}

	last_button_state_exit = reading_exit_button;

	// car exit - ends

	//debug info

	stat = 3;
}

void car_entrance3() {

	//car entrance - starts

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

	if (Sensor[0].ping_cm() < 4) {
		green_led_off(2);
		red_led_on(0);
	}
	else {
		green_led_on(2);
		red_led_off(0);
	}

	last_button_state = reading_entry_button;
	
	 // car entrance - ends

	// car exits - starts

	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();

	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				state = empty;
				total_parking = total_parking - 1;
			}
		}
	}


	last_button_state_exit = reading_exit_button;

	// car exits - ends

	stat = 3;

}

void full() {

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

	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();

	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				state = empty;
				total_parking = total_parking - 1;
			}
		}
	}


	last_button_state_exit = reading_exit_button;

	//display screen results
	stat = 4;
}