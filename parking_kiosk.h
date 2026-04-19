/*
 * Shared declarations for Smart Parking Kiosk (hardware pass 4).
 */
#ifndef PARKING_KIOSK_H
#define PARKING_KIOSK_H

#include <Arduino.h>
#include <stdint.h>
#include <LiquidCrystal.h>
#include <NewPing.h>

#define mask(x) ((uint8_t)(1 << x))
#define MAX 6
#define MIN 0

extern LiquidCrystal lcd;
extern NewPing Sensor[MAX];
extern void (*state)();

extern const uint8_t entry_button;
extern short button_state;
extern int last_button_state;
extern unsigned long LastDebounceTime;

extern const uint8_t exit_button;
extern short button_state_exit;
extern int last_button_state_exit;
extern unsigned long LastDebounceTimeExit;

extern const unsigned long Debounced_Delay;

extern unsigned short total_parking_spot;
extern uint8_t total_parking;

extern const int dip_switch_pin;
extern int dip_switch;

extern uint8_t debug_state;

void kiosk_setup(void);
void kiosk_loop(void);

void time_delay_func(unsigned long, const uint16_t);
void lcd_display_init(void);
void lcd_display_func(int);
void lcd_display_full(void);
void lcd_display_empty(void);
void lcd_display_position(char*);

void green_led_on(const uint8_t);
void green_led_off(const uint8_t);
void red_led_on(const uint8_t);
void red_led_off(const uint8_t);

void exit_button_func(void);

void empty_a(void);
void car_entrance1_a(void);
void car_entrance2_a(void);
void car_entrance3_a(void);
void car_entrance4_a(void);
void car_entrance5_a(void);
void car_entrance6_a(void);

void empty_b(void);
void car_entrance1_b(void);
void car_entrance2_b(void);
void car_entrance3_b(void);
void car_entrance4_b(void);
void car_entrance5_b(void);
void car_entrance6_b(void);

void full(void);

#endif
