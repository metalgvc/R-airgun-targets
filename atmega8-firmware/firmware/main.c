#include <avr/io.h>

#define F_CPU 8000000UL

int TIMER_MODE = 0;
int COUNTER_MODE = 1;

/*
 * set mode
 * 0 - TIMER - DONE
 * 1 - COUNTER -
 */
int MODE = 0;

int PAUSE = 1;
uint32_t PAUSE_LAST_TCOUNTER = 0;

#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.c"
#include "mylibs/timer.c"
#include "mylibs/SPI.c"
#include "mylibs/display.c"

#include "mylibs/core.c"
#include "TIMER_MODE.c"
#include "COUNTER_MODE.c"

void init(void);
void check_start_switch_mode_btn(void);

// debounce last time
uint32_t _swtchBtnLastTime = 0;
int _swtchBtnLastState = 0;

// ------- code --------

// init
void init(){
    sei(); // enable interrupts
    
    SPI_init();
    display_init();
    
    // set PORTC as input for target buttons
    //DDRC = 0b00000000; // 0 - input; 1 - output
    // set PC0-5 as input
    DDRC &= ~(1<<PC0) & ~(1<<PC1) & ~(1<<PC2) & ~(1<<PC3) & ~(1<<PC4) & ~(1<<PC5);
    PORTC |= (1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3) | (1<<PC4) | (1<<PC5); // PC0-5 high
    
    // set PORTD as output
    DDRD = 0b11111111;
    PORTD = 0b00000000; // set low on pins
    
    // set PINB0 for start/reset mode button as input
    DDRB &= ~(1<<PB0); // input
    PORTB |= (1<<PB0); // high
    
    init_timer();
    init_counter();
    
    // up targets
    //up_all_targets();
}


// main
int main(void)
{
    init();
    
    while (1) {
        
        check_start_switch_mode_btn();
        
        if (PAUSE == 1) {
            continue;
        }
        
        check_button(0, PC0); // btn1
        check_button(1, PC1); // btn2
        check_button(2, PC2); // btn3
        check_button(3, PC3); // btn4
        check_button(4, PC4); // btn5
        check_button(5, PC5); // btn6
        
// debug code ========================
//if (TCOUNTER > 1000) {
//    num1 = random_at_most(1);
//    num2 = random_at_most(2);
//    num3 = random_at_most(5);
//    num4 = random_at_most(9);
//    TCOUNTER = 0;
//}
//
//show_number();
//num1 = 1;
//num2 = 2;
//num3 = 3;
//num4 = 4;
//show_number();
//continue;
// end - debug code ==================
        
        // TIMER MODE
        if (MODE == TIMER_MODE) {
            
            main_timer();
            
        // COUNTER MODE
        } else if (MODE == COUNTER_MODE) {
            main_counter();
        }
        
    }
    
    return 0;
}

// reset/start/switch mode button
void check_start_switch_mode_btn(){
    
    // read reset/start btn
    if (GETBIT(PINB, PB0) == 0) { // pressed
        
        // dbounce counter
        if (_swtchBtnLastState == 0) {
            _swtchBtnLastState = 1;
            _swtchBtnLastTime = TCOUNTER;
        } else if (TCOUNTER > (_swtchBtnLastTime + 200 /*ms*/) ) {
            _swtchBtnLastTime = TCOUNTER; // switch mode each 200ms on btn pressed
            
            // handle PRESSED btn
            PAUSE = 1;
            PAUSE_LAST_TCOUNTER = TCOUNTER;
            int maxMode = 1;
            MODE++;
            if (MODE > maxMode) {
                MODE = 0;
            }
        }
    } else {
        // released btn
        _swtchBtnLastState = 0;
    }

    if (PAUSE == 1) {
        // show mode on display
        num1 = MODE; num2 = 0; num3 = 8; num4 = 8;
        show_number();
        
        // wait 3 second
        if ( (TCOUNTER - PAUSE_LAST_TCOUNTER)/TCOUNTER_DIVIDER > 3 ) {
            PAUSE = 0;
            num1 = 0; num2 = 0; num3 = 0; num4 = 0;
            
            if (MODE == TIMER_MODE) {
                init_timerm();
                TIMER_STARTED = 0;
            } else if (MODE == COUNTER_MODE) {
                init_counter();
                COUNTER = 0;
            }

        }
    }
}
