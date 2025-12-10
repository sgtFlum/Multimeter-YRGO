/*
 * Drivers.c
 *
 * Created: 2025-11-04 14:47:17
 * Author: Ludvig
 */ 

#include "P2.h"

void setup_p2(void) {
    /*sätter i/o för PORTB
    Bit     Arduino pin     Funktion        i/o 1 = out
    0       pin 8           Funktionsval 2  0
    1       pin 9           Funktionsval 3  0
    2       pin 10          Led             1
    3       pin 11          Digit 1         1
    4       pin 12          Digit 2         1
    5       pin 13          Digit 3         1
    6       crystal         Används ej      0
    7       crystal         Används ej      0

    bit      76543210     */
    DDRB = 0b00111100;    
    /*sätter i/o för PORTC
    Bit     Arduino pin     Funktion        i/o 1 = out
    0       pin A0          Volt låg        0
    1       pin A1          Volt hög        0
    2       pin A2          Resistans låg   0
    3       pin A3          Resistans hög   0
    4       pin A4          Batterikontroll 0
    5       pin A5          Funktionsval 4  0               
    6       reset           Används ej      0
    7       ---             Används ej      0
    bit      76543210     */
    DDRC = 0b00000000;
    /*sätter i/o för PORTD
    Bit     Arduino pin     Funktion        i/o 1 = out
    0       pin 0           Beeper          1
    1       pin 1           Funktionsval 5  0
    2       pin 2           BCD 0           1
    3       pin 3           BCD 1           1
    4       pin 4           BCD 2           1
    5       pin 5           BCD 3           1
    6       pin 6           decimalpunkt    1
    7       pin 7           Funktionsval 1  0
    bit      76543210       */
    DDRD = 0b01111101;

}

/************************************************************************/
/* bool read_digital_input(uint8_t port,uint8_t bit)                    */
/*                                                                      */
/* Argument:                                                            */
/* uint8_t bit                                                          */
/* Representerar den bit på PORTB som skall läsas av                    */
/*                                                                      */
/* Argument:                                                            */
/* uint8_t port                                                         */
/* Representerar den villen av portb/portc/portd som använda            */
/*                                                                      */
/* Return:                                                              */
/* True eller False beroende på om ingången är hög eller låg            */
/************************************************************************/
bool read_digital_input(uint8_t port,uint8_t bit) {
    if (port == 1){
        return PINB & (1 << bit);    
    } else if (port == 2) {
        return PINC & (1 << bit); 
    } else {
        return PIND & (1 << bit);        
    } 
}

/************************************************************************/
/* bool write_digital_output_portd(uint8_t bit, bool value)             */
/*                                                                      */
/* Argument:                                                            */
/* uint8_t port                                                         */
/* Representerar om PORTB/PORTC/PORTD ska användas                      */
/*                                                                      */
/* Argument:                                                            */
/* uint8_t bit                                                          */
/* Representerar den bit på PORTD som skall skrivas till                */
/*                                                                      */
/* bool value                                                           */
/* true eller false beroende på om man vill sätta utgången till hög/låg */
/*                                                                      */
/* Return:                                                              */
/* Void                                                                 */
/************************************************************************/

void write_digital_output(uint8_t port, uint8_t bit, bool value) {
    if (port == 1) {
        if(value) {
            PORTB |= (1 << bit);
            }else {
            PORTB &= ~(1 << bit);
        }    
    } else if (port == 2){
        if(value) {
            PORTC |= (1 << bit);
            }else {
            PORTC &= ~(1 << bit);
        }
    } else {
        if(value) {
            PORTD |= (1 << bit);
            }else {
            PORTD &= ~(1 << bit);
        }       
    }
}


/************************************************************************/
/* uint16_t read_analogue_input(uint8_t input_number)                   */
/*                                                                      */
/* Argument:                                                            */
/* uint8_t input_number                                                 */
/* Representerar den ingång (0 - 5) som skall läsas av                  */
/*                                                                      */
/* Return:                                                              */
/* Det returnerade värdet är ett 16-bitars uint-tal mellan 0 - 1023     */
/************************************************************************/
uint16_t read_analogue_input(uint8_t input_number) {
    ADMUX = (input_number);
    ADCSRA = ((1 << ADEN) | (1 << ADSC) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));
    while ((ADCSRA & (1 << ADIF)) == 0) ;
    ADCSRA = (1 << ADIF);
    return ADC;
}

/************************************************************************/
/* bool display(uint16_t value, uint8_t digit)                          */
/*                                                                      */
/* PORTD bit  Funktion                                                  */
/* 0                                                                    */
/* 1                                                                    */
/* 2          BCD A                                                     */
/* 3          BCD B                                                     */
/* 4          BCD C                                                     */
/* 5          BCD D                                                     */
/* 6                                                                    */
/* 7                                                                    */
/*                                                                      */
/* PORTB bit  Funktion                                                  */
/* 0                                                                    */
/* 1                                                                    */
/* 2                                                                    */
/* 3          Gemensam katod siffra 0                                   */
/* 4          Gemensam katod siffra 1                                   */
/* 5          Gemensam katod siffra 2                                   */
/* 6                                                                    */
/* 7                                                                    */
/*                                                                      */
/* Argument:                                                            */
/* uint16_t value                                                       */
/* Värdet som skall visas på displayen 0 - 999                          */
/*                                                                      */
/* uint8_t digit                                                        */
/* Siffran som skall visas, 0 - 2 för att visa alla tre siffror använd  */
/* en for-loop och loopa igenom 0 - 2                                   */
/*                                                                      */
/* Return:                                                              */
/* bool, false om värdet är högre än 999, true om talet går att visa.   */
/************************************************************************/


bool display(uint16_t value, uint8_t digit, bool decimal){
    uint8_t value_digit[3];
    uint8_t portd = 0;
    if(decimal){
        portd |= 0b01000000;
    }else{
        portd &= ~0b01000000;
    }  
    uint8_t portb = PORTB;

    if(value > 999){
        return false;
    }else{
        /* Breaks down the value */
        value_digit[2] = value%10;      /* Singular */
        value_digit[1] = value/10%10;   /* Tens     */
        value_digit[0] = value/100%10;  /* Hundreds */
        
        
        
        portb |= 0b00111000;
        portb &= ~(1<<(digit+3));
        
        portd &= ~(0b00111100);
        portd |= (value_digit[digit] << 2) & 0b00111100;
        PORTD = portd;
        
        PORTB = portb;
        return true;
    }
}
