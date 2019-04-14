/*
 code: hardware pass 4 revision
 date: 04/15/2019
 developer: Yasir Jamal
 uchl id: 1141556
 project: Spring Captsone - Smart Parking Kiosk
*/

//libraries are included to support integers, lcd display and ultra-motion sensors
#include <stdint.h>
#include <LiquidCrystal.h>
#include <NewPing.h>

//below macros are added to support bit manipulation
#define mask(x) ((uint8_t) (1 << x))
#define MAX 6		//number of car parking spots
#define MIN 0		//parking is empty

//user defined function for time delay
void time_delay_func(unsigned long, const uint16_t);

//user defined function for lcd
void lcd_display_init();
void lcd_display_func(int);
void lcd_display_full(void);
void lcd_display_empty(void);
void lcd_display_position(char*);

//user defined function for LEDs i.e. Green and Red
void green_led_on(const uint8_t);
void green_led_off(const uint8_t);
void red_led_on(const uint8_t);
void red_led_off(const uint8_t);

//state machine function pointer
void (*state)();

//state machine function from level 1 to level 2
void empty_a();
void car_entrance1_a(); //1A
void car_entrance2_a(); //1B
void car_entrance3_a(); //1C
void car_entrance4_a(); //2A
void car_entrance5_a(); //2B
void car_entrance6_a(); //2C


//state machine function from level 2 to level 1
void empty_b();
void car_entrance1_b(); //2A
void car_entrance2_b(); //2B
void car_entrance3_b(); //2C
void car_entrance4_b(); //1A
void car_entrance5_b(); //1B
void car_entrance6_b(); //1C
//void full_b();

//state machine function when the parking lot is full
//shared by both mode of dip switches i.e. low or high
void full();

//user defined function for exit event
//this can be used in both modes
void exit_button_func();

//setup for LCD Display
//TODO - Review the digital LED position
const uint8_t rs = 12;
const uint8_t en = 11;
const uint8_t d4 = 5;
const uint8_t d5 = 4;
const uint8_t d6 = 3;
const uint8_t d7 = 2;

//object instantiation for LCD display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//setup for Entry Push Button
//TODO - review the digital positon for the input signal
const uint8_t entry_button = 52;
short button_state;
int last_button_state = HIGH;
unsigned long LastDebounceTime = 0;

//setup for Exit Push Button
//TODO - review the digital positon for the input signal
const uint8_t exit_button = 50;
short button_state_exit;
int last_button_state_exit = HIGH;
unsigned long LastDebounceTimeExit = 0;

//debounce delay is set to 50ms
//TODO - may need to change the vale to 25ms for faster response
const unsigned long Debounced_Delay = 50;


//global variables for parking position
//TODO - These may not be needed in the design
unsigned short total_parking_spot = MAX;
uint8_t total_parking = 0;

//setting ultra-motion sensor pins
//TODO - review all the six sensor digital pins
const uint8_t trig1 = 8;  //1A
const uint8_t echo1 = 9;  //1A
const uint8_t trig2 = 6;  //1B
const uint8_t echo2 = 7;  //1B
const uint8_t trig3 = 13; //1C
const uint8_t echo3 = 10; //1C
const uint8_t trig4 = 48; //2A
const uint8_t echo4 = 49; //2A
const uint8_t trig5 = 46; //2B
const uint8_t echo5 = 47; //2B
const uint8_t trig6 = 44; //2C
const uint8_t echo6 = 45; //2C

//creating sensor objects using sensor library
//TODO - check sensor pins
//third parameter i.e. 500 or x is the max distance to poll
NewPing Sensor[MAX] = {
	NewPing(trig1, echo1, 500),
	NewPing(trig2, echo2, 500),
	NewPing(trig3, echo3, 500),
	NewPing(trig4, echo4, 500),
	NewPing(trig5, echo5, 500),
	NewPing(trig6, echo6, 500)
};

//creating dip switch variable
//TODO - review the digital input signal
const int dip_switch_pin = 40;
int dip_switch;

//debug variables
uint8_t debug_state;

//creating the setup function
//TODO - review it for setting up the pinMode
void setup() {

	//open up the serial port with 9600 baud rate to verify debug info
	Serial.begin(9600);

	//initializing the display on the LCD screen
	lcd_display_init();

	//setting up entry button
	pinMode(entry_button, INPUT);
	pinMode(exit_button, INPUT);

	//setup for red led on six parking stations and setting it up as OUTPUT
	//digital signals 29-22 [MSB - LSB]
	//PORTA 
	//TODO - review 
	DDRA = mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);

	// breadboard pin location on red board
	/* 14 - 1A
	   13 - 1B
	   12 - 1C
	   10 - 2A
	   09 - 2B
	   08 - 2C*/

	   //setup for green led on six parking stations and setting it up as OUTPUT
	   //digital signals 30-37 [MSB - LSB]
	   //PORTC
	   //TODO -re view
	DDRC = mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);

	// breadboard pin location on red board
	/* 22 - 1A  - 37
	   21 - 1B	  36
	   20 - 1C	  35
	   18 - 2A    34
	   17 - 2B    33
	   16 - 2C    32
	   */

	   //initialize the green LED and set it to OFF
	PORTC |= mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);

	//initialize the gred LED and set it to OFF
	PORTA |= mask(5) | mask(4) | mask(3) | mask(2) | mask(1) | mask(0);

	//setting up pin on the DIP switch
	pinMode(dip_switch_pin, INPUT);
	dip_switch = digitalRead(dip_switch_pin);

	//dip switch is read during the power on state of the controller
	// LOW - state machine starts from 1A to 2C
	// HIGH - state machine starts from 2A to 1C

	// code break
	if (dip_switch == LOW) {
		state = empty_a;
	}
	else if (dip_switch == HIGH) {
		state = empty_b;
	}

	//state = empty_a;

}

//main controller function

void loop() {

	//state machine loops infinitely
	state();

	//debug variables to be included in this section
	//to display data on COM port
	Serial.print("State: ");
	Serial.println(debug_state);

}

//time delay function using built-in arduino library
void time_delay_func(unsigned long time_now, const uint16_t time_interval) {
	time_now = millis();
	while (millis() < time_now + time_interval) {
		// time delay in ms
	}
}

//lcd display user defined function
//this display the number of cars in the parking
//may not be needed as we are using the state machine model
//useful for debugging and display it on the COM port via serial commands
void lcd_display_func(int data) {
	lcd.clear();
	lcd.setCursor(0, 1);
	lcd.print("Park Avail:");
	lcd.setCursor(11, 1);
	lcd.print(data);
}

//user defined function to display that parking lot is now full or the state machnine is in full function
void lcd_display_full(void) {
	lcd.clear();
	lcd.setCursor(0, 1);
	lcd.print("Parking FULL!");
}

//function to display empty on the lcd screen when the state machine is at empty pointer
void lcd_display_empty(void) {
	lcd.clear();
	lcd.setCursor(0, 1);
	lcd.print("     EMPTY!   ");
}

//function to display the position of the parking spot where the LED will turn green to allow the car to park there
void lcd_display_position(char* pos) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Park at position: ");
	lcd.setCursor(0, 1);
	lcd.print(pos);
}

//function to display the initial information once the controller starts and car parking positions available before any car is allowed to enter
void lcd_display_init() {
	lcd.clear();
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("Park Avail:");
	lcd.setCursor(11, 0);
	lcd.print(MAX);
}


//function to turn ON and OFF, Green and Red LEDs
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

//the below function is going to get trigger everytime the exit button is pressed
//this can be used with both version of state machines to place the state machine..
//to either empty_a or empty_b function depending on the position of dip switch

void exit_button_func() {
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

//below is empty function for state machine from 1A to 2C, this means...
//there are either no car in the parking or an already parked car just left..
//the parking lot
//DIP position low
void empty_a() {
	//push button for car entrance pushed
	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}
	//debounce delay
	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO
				//first sensor of parking level 1
				if (Sensor[0].ping_cm() > 10) {			//check if there is car
					total_parking = total_parking + 1;  //add the cars tallly
					lcd_display_position("1A");			//park at position A
					state = car_entrance1_a;			//move car to next state
				}
				else {
					state = car_entrance2_a;			//if the car is parked at entrance1, then jump to station2
				}
			}

		}

	}

	last_button_state = reading_entry_button;

	//push button for car exit pushed
	short reading_exit_button = digitalRead(exit_button);

	if (reading_exit_button != last_button_state_exit) {
		LastDebounceTimeExit = millis();
	}

	if (millis() - LastDebounceTimeExit > Debounced_Delay) {

		if (reading_exit_button != button_state_exit) {
			button_state_exit = reading_exit_button;

			if (button_state_exit == LOW) {
				state = empty_a;
				//lcd_display_empty();
				if (total_parking > 0) {
					total_parking--;		//decrement the car counter..
				}							//everytime the exit button is pushed
				if (total_parking == 0) {
					lcd_display_empty();
				}
			}
		}
	}

	last_button_state_exit = reading_exit_button;

	//resetting the sensors in empty state and turn off LEDs if the car...
	//is not parked
	for (uint8_t i = 0; i < MAX; i++) {
		if (Sensor[i].ping_cm() > 10) {
			green_led_off(i);
			red_led_off(i);
		}
	}

	//car parking available
	//lcd_display_func(MAX - total_parking);

	//debug info
	Serial.println(Sensor[0].ping_cm());
	debug_state = 0;

}

//below is car parking position 1A for state machine from 1A to 2C...
//state machine will move from empty_a to car_entrance1_a if entrance button...
//is pushed and if there is no car in parking position 1A
//DIP position low
void car_entrance1_a() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for second spot on level 1
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 1A 
	if (Sensor[0].ping_cm() < 10) {
		green_led_off(0);
		red_led_on(0);
	}
	else {
		green_led_on(0);
		red_led_off(0);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	Serial.println(Sensor[1].ping_cm());
	debug_state = 1;
}

//from 1B to 1C when entrance button is pushed
//dip switch state is LOW
void car_entrance2_a() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for third spot on level 1
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 1B
	if (Sensor[1].ping_cm() < 10) {
		green_led_off(1);
		red_led_on(1);
	}
	else {
		green_led_on(1);
		red_led_off(1);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	Serial.println(Sensor[2].ping_cm());
	debug_state = 2;

}

//from 1C to 2A
//dip switch state is LOW
void car_entrance3_a() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for first spot on level 2 i.e. 2A
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 1C
	if (Sensor[2].ping_cm() < 10) {
		green_led_off(2);
		red_led_on(2);
	}
	else {
		green_led_on(2);
		red_led_off(2);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 3;
}

//from 2A to 2B
//dip switch state is LOW
void car_entrance4_a() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for second spot on level 2 i.e. 2B
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 2A
	if (Sensor[3].ping_cm() < 10) {
		green_led_off(3);
		red_led_on(3);
	}
	else {
		green_led_on(3);
		red_led_off(3);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 4;
	Serial.print("Sensor Station 2B: ");
	Serial.println(Sensor[4].ping_cm());

}

//from 2B to 2C
//dip switch state is LOW
void car_entrance5_a() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for third spot on level 2 i.e. 2C
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 2B
	if (Sensor[4].ping_cm() < 10) {
		green_led_off(4);
		red_led_on(4);
	}
	else {
		green_led_on(4);
		red_led_off(4);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 5;
}

//from 2C to Full
//dip switch state is LOW
void car_entrance6_a() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				/*if (Sensor[5].ping_cm() > 10) {
					state = car_entrance6_a;
					total_parking = total_parking + 1;
					lcd_display_position("2C");
					green_led_on(5);
				//}
				else {
					state = full;
				} */
				lcd_display_full();
				state = full;
				
			}

		}

	}

	//TODO - review sensor and LED
	//this is the sensor of the parking position 2C
	if (Sensor[5].ping_cm() < 10) {
		green_led_off(5);
		red_led_on(5);
	}
	else {
		green_led_on(5);
		red_led_off(5);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 6;
}

//state machine at FULL
//this can be shared with both "a" and "b" state machines
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

	exit_button_func();

	//debug info
	debug_state = 7;
}

// code break

//below is empty function for state machine from 2A to 1C, this means...
//there are either no car in the parking or an already parked car just left..
//this state machine is dependent on dip switch position 
//dip switch position needs to be HIGH for this state
void empty_b() {
	//push button for car entrance pushed
	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}
	//debounce delay
	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor positions
				//first sensor of second level 2A
				if (Sensor[3].ping_cm() > 10) {			//check if there is car
					lcd_display_position("2A");			//park at position A
					state = car_entrance1_b;			//move car to next state
					total_parking = total_parking + 1;  //add the cars tallly
					
					//TODO - review LED positions
				}
				else {
					state = car_entrance2_b;			//if the car is parked at entrance1, then jump to station2
				}
			}

		}

	}

	last_button_state = reading_entry_button;

	//push button for car exit pushed
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
					total_parking--;		//decrement the car counter..
				}							//everytime the exit button is pushed
			}
		}
	}

	last_button_state_exit = reading_exit_button;

	//resetting the sensors in empty state and turn off LEDs if the car...
	//is not parked
	for (uint8_t i = 0; i < MAX; i++) {
		if (Sensor[i].ping_cm() > 10) {
			green_led_off(i);
			red_led_off(i);
		}
	}

	//debug info

	debug_state = 10;

}

//below is car parking position 2A for state machine from 2A to 1C...
//state machine will move from empty_b to car_entrance1_b if entrance button...
//is pushed and if there is no car in parking position 2A
//this state machine is dependent on dip switch position 
//dip switch position needs to be HIGH for this state
//from empty to 2A
void car_entrance1_b() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for second spot on level 2
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 2A 
	if (Sensor[3].ping_cm() < 10) {
		green_led_off(3);
		red_led_on(3);
	}
	else {
		green_led_on(3);
		red_led_off(3);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info

	debug_state = 11;

}

//from 2A to 2B
//dip switch mode is HIGH
void car_entrance2_b() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for third spot on level 2
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 2B
	if (Sensor[4].ping_cm() < 10) {
		green_led_off(4);
		red_led_on(4);
	}
	else {
		green_led_on(4);
		red_led_off(4);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 12;
}

//from 2B to 2C
//dip switch mode is HIGH
void car_entrance3_b() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for first spot on level 1
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 2C
	if (Sensor[5].ping_cm() < 10) {
		green_led_off(5);
		red_led_on(5);
	}
	else {
		green_led_on(5);
		red_led_off(5);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 13;

}

// from 2C to 1A
void car_entrance4_b() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for second spot on level 1
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 1A
	if (Sensor[0].ping_cm() < 10) {
		green_led_off(0);
		red_led_on(0);
	}
	else {
		green_led_on(0);
		red_led_off(0);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 14;

}

// from 1A to 1B
void car_entrance5_b() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for third spot on level 1
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

	//TODO - review sensor and LED
	//this is the sensor of the parking position 1B
	if (Sensor[1].ping_cm() < 10) {
		green_led_off(1);
		red_led_on(1);
	}
	else {
		green_led_on(1);
		red_led_off(1);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 15;

}

//from 1B to 1C
void car_entrance6_b() {

	short reading_entry_button = digitalRead(entry_button);

	if (reading_entry_button != last_button_state) {
		LastDebounceTime = millis();
	}

	if (millis() - LastDebounceTime > Debounced_Delay) {

		if (reading_entry_button != button_state) {
			button_state = reading_entry_button;

			if (button_state == LOW) {
				//TODO - review sensor and LED pins
				//sensor for third spot on level 1
				/*if (Sensor[2].ping_cm() > 10) {
					state = car_entrance5_b;
					total_parking = total_parking + 1;
					lcd_display_position("1C");
					green_led_on(2);
				}
				else {
					state = full;
				} */
				lcd_display_full();
				state = full;
			}

		}

	}

	//TODO - review sensor and LED
	//this is the sensor of the parking position 1C
	if (Sensor[2].ping_cm() < 10) {
		green_led_off(2);
		red_led_on(2);
	}
	else {
		green_led_on(2);
		red_led_off(2);
	}

	last_button_state = reading_entry_button;

	//exit button pushed
	exit_button_func();

	// Debug Info
	debug_state = 16;

}




// Bug Fixes
// 1. update the display feature at each state after the car leaves.


