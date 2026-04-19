/*
 Name:		Second Hardware Pass Code
 Created:	03/30/2019
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
void lcd_display_func(int);
void lcd_display_full(void);
void lcd_display_empty(void);
float ultra_motion_sensor(const int, const int);
void green_led_on(const uint8_t);
void green_led_off(const uint8_t);
void red_led_on(const uint8_t);
void red_led_off(const uint8_t);

// Setup for LCD Display
const uint8_t rs = 12;
const uint8_t en = 11;
const uint8_t d4 = 5;
const uint8_t d5 = 4;
const uint8_t d6 = 3;
const uint8_t d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


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

//parking matrix integer
uint8_t parking_spot[MAX] = {};

//setting ultra-motion sensor pins

const uint8_t trig1 = 8;
const uint8_t echo1 = 9;
const uint8_t trig2 = 6;
const uint8_t echo2 = 7;
const uint8_t trig3 = 13;
const uint8_t echo3 = 10;

//the setup function runs once when you press reset or power the board
void setup() {

	//opening up the serial port
	Serial.begin(9600);

	//setting up LCD display
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("Park Total:");
	lcd.setCursor(11, 0);
	lcd.print(MAX);

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

	//setting up ultra-motion sensor pins mode
	pinMode(trig1, OUTPUT);
	pinMode(trig2, OUTPUT);
	pinMode(trig3, OUTPUT);
	pinMode(echo1, INPUT);
	pinMode(echo2, INPUT);
	pinMode(echo3, INPUT);

	// Set the GREEN and RED LED OFF
	PORTA |= mask(2) | mask(1) | mask(0); 
	PORTC |= mask(2) | mask(1) | mask(0); 
}

//the loop function runs over and over again until power down or reset
void loop() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if ((button_state == LOW) && (total_parking_spot > MIN)) {
				total_parking_spot--;
				lcd_display_func(total_parking_spot);
				green_led_on(total_parking_spot);
			}

			else if ((button_state == LOW) && (total_parking_spot == MIN)) {
				lcd_display_full();
			}

		}
	}

	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();

	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			// code for decrement
			if ((button_state_exit == LOW) && (total_parking_spot < MAX) && (total_parking_spot >= MIN)) {
				green_led_off(total_parking_spot);
				total_parking_spot++;
				lcd_display_func(total_parking_spot);

			}

			else if ((button_state_exit == LOW) && (total_parking_spot == MAX)) {
				lcd_display_empty();
			}

		}
	}

	last_button_state = reading_entry_button;
	last_button_state_exit = reading_exit_button;


	//serial output on COM port
	Serial.print("Parking Available: ");
	Serial.println(total_parking_spot);

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

//measuring distance from the sensor

float ultra_motion_sensor(const int trigger, const int echo) {
	long distance;
	digitalWrite(trigger, LOW);
	delayMicroseconds(2);
	digitalWrite(trigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigger, LOW);
	distance = pulseIn(echo, HIGH);
	return distance;
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

