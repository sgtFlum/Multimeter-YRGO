/*
* P2-multimeter.c
*
* Created: 2025-11-04 14:44:46
* Author : Ludvig
*/

#include "P2.h"

int main(void) {
    
    setup_p2();
    uint32_t timer_counter = 0;
    battery();
    /*for (uint8_t i = 0; i<200; i++)  { Denna kod är till för att visa versionsnummr i 2 sekunder, denna används inte utan är endast här enligt kravspec. itsället så visas batteri nivån under uppstart.
        show_number(101,0);
    }*/
    while (1) {
        
        if (read_digital_input(PORT_D,7)==true) { /*checks pins for input and runs correct function for that input*/
            low_v();
            timer_counter=0;
            
        } else if (read_digital_input(PORT_B,0)==true) {
            hi_v();
            timer_counter=0;
            
        } else if (read_digital_input(PORT_B,1)==true) {
            low_r();
            timer_counter=0;
            
        } else if (read_digital_input(PORT_C,5)==true) {
            hi_r();
            timer_counter=0;
            
        } else if (read_digital_input(PORT_D,1)==true) {
            buzzer();
            timer_counter=0;
            
        } else {
            timer_counter = timer(timer_counter);
            
        }        
    }
}