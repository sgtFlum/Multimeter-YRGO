/*
 * Functions.c
 *
 * Created: 2025-11-07 11:51:33
 * Author: Ludvig
 */ 
#include "P2.h"

/* controls LED to indicate low leads*/
void low_led(bool state) {
    write_digital_output(PORT_B,2,state);
}

uint16_t length_of_uint(uint32_t number){
    uint8_t length = 0;
    while(number>0){
        number = number/10;
        length++;
    }
    return length;
}

bool show_number (uint16_t number,  uint8_t decimal) { //har en 10 MS wait!!!
    if (number > 999) {
        return false;
    } else {
        for (uint8_t i = 0; i<3; i++) {
                if (decimal == i && i!=2) {
                    display(number,i,true);
                } else {
                    display(number,i,false);
                }
            _delay_ms(3);
        }
        _delay_ms(1);
        return true;
    }
}

void show_decimal (bool point1, bool point2, bool point3){
           write_digital_output(PORT_D,6,true);
           write_digital_output(PORT_D,5,true);
           write_digital_output(PORT_D,4,true);
           write_digital_output(PORT_D,3,true);
           write_digital_output(PORT_D,2,true);
           
           write_digital_output(PORT_B,3,!point1);
           write_digital_output(PORT_B,4,!point2);
           write_digital_output(PORT_B,5,!point3);
}

void low_v(void) {
    low_led(true);

    float low_v_f = read_analogue_input(0)*(5.0/1024.0);
    uint16_t low_v_i = low_v_f*100.0;
    uint8_t decimal = 2 - (length_of_uint((uint32_t)(low_v_i/low_v_f)));
    show_number(low_v_i,decimal);
    
} // lösa medelvärde av 10 mätningar?

void hi_v(void) {
    low_led(false);
    
    float hi_v_f = read_analogue_input(1)*(5.0/1024.0)*(13.58/1.77);
    uint16_t hi_v_i = hi_v_f*10;
    
    if (hi_v_i <99){
        hi_v_i = hi_v_f * 100; // try hi_v_i = hi_v_f * 100; o se om man kan läsa under 100 mV
    }
    
    uint8_t decimal = 2 - (length_of_uint((uint32_t)(hi_v_i/hi_v_f)));
    show_number(hi_v_i,decimal);
}

void low_r(void){
    low_led(true);
    
    float avg_low_r = 0;
    for (uint8_t i = 0; i< 10; i++) {
        float low_r_read = (read_analogue_input(2)*(5.0/1024.0)*269.5/(5.0-(read_analogue_input(2)*(5.0/1024.0))));
        avg_low_r = avg_low_r +low_r_read;
    }
    float low_r_f = avg_low_r / 10;
    
    uint16_t low_r_i = low_r_f;
    
    if (low_r_i <10) {
        low_r_i=low_r_f*100;
    }
    if (low_r_i <100) {
        low_r_i=low_r_f*10;
    }
    
    uint8_t decimal =2 - length_of_uint((uint32_t) (low_r_i/low_r_f));
    show_number(low_r_i,decimal);
}

void hi_r(void) {
    low_led(true);
    
    float avg_hi_r = 0;
    for (uint8_t i = 0; i< 10; i++)  {
        float hi_r_read = (read_analogue_input(3)*(5.0/1024.0)*26.69/(5.0-(read_analogue_input(3)*(5.0/1024.0))));
        avg_hi_r = avg_hi_r + hi_r_read;
    }
    float hi_r_f = avg_hi_r/10;
    
    uint32_t hi_r_i = hi_r_f;
    
    if (hi_r_i <10) {
        hi_r_i=hi_r_f*100;
    }
    if (hi_r_i <100) {
        hi_r_i=hi_r_f*10;
    }
    
    uint8_t decimal =2 - length_of_uint((uint32_t) (hi_r_i/hi_r_f));
    show_number(hi_r_i,decimal);
}

void buzzer(void) {
    low_led(true);
    show_decimal(false,false,false);
    if (read_analogue_input(2)<32){
        show_decimal(true,true,true);
        for (uint16_t i=0; i<100; i++){
            if (i < 10){
                write_digital_output(PORT_D,0,true);
            } else {
                write_digital_output(PORT_D,0,false);
            }
        }
    }
}

/*requires a counter variable in main*/
uint32_t timer(uint32_t timer_counter) {
    low_led(false);
    
    if (timer_counter>100000) {
        timer_counter = 0;
    }
    
    if ((timer_counter%10000)==6000) {
        timer_counter = timer_counter+4000;
    }
    
    if (timer_counter%10 <  3) {
        show_number((timer_counter/100),0);
        } else {
        show_number((timer_counter/100),2);
    }
    
    timer_counter++;
    return timer_counter;
} 

void battery(void){ //lyser alla tre punkter är batteriet bra, två mid, blinkar en byt batteri
    low_led(false);
    
    float battery_f = read_analogue_input(4);
    if (battery_f < 669) { // low batery get correct value below 8v? går o göra utan wait?
        for (uint8_t i=0; i<10; i++){
            show_decimal(false,false,true);
            _delay_ms(100);
            show_decimal(false,false,false);
            _delay_ms(100);
        }
        } else if (battery_f < 754) { // ok battery get correct value 8-9v?
        show_decimal(false,true,true);
        _delay_ms(2000);
        } else if (battery_f < 925){ // full bat
        show_decimal(true,true,true);
        _delay_ms(2000);
        } else { // DC power
        show_decimal(false,false,false);
    }
}