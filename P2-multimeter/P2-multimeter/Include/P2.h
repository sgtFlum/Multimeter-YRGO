/*
 * P2.h
 *
 * Created: 2025-11-04 14:57:30
 * Author: Ludvig
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <util/delay.h>


#define  PORT_B 1
#define  PORT_C 2
#define  PORT_D 3

void setup_p2(void);
void write_digital_output(uint8_t port, uint8_t bit, bool value);
bool display(uint16_t value, uint8_t digit, bool decimal);
uint16_t read_analogue_input(uint8_t input_number);
bool read_digital_input(uint8_t port,uint8_t bit);
bool show_number (uint16_t number,  uint8_t decimal);

void battery(void);
void low_v(void);
void hi_v(void);
void low_r(void);
void hi_r(void);
void buzzer(void);
uint32_t timer(uint32_t timer_counter);

void low_led(bool state);
uint16_t length_of_uint(uint32_t number);
void show_decimal (bool point1, bool point2, bool point3);

#endif /* INCFILE1_H_ */