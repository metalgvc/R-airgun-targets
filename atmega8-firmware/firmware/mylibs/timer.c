//
//  timer.c
//  targets
//
//  Created by metal gvc on 9/9/18.
//

unsigned char TIMER;
int TIMER_STARTED = 0;
int TIMER_PAUSED = 1;
uint32_t TCOUNTER = 0;
int TCOUNTER_DIVIDER = 1000;

void init_timer(void);

// TIMER1 init
void init_timer(){
    
    // frequency 8000000/64=125000 (Hz) per second
    // 0b00000000H 0b01111101L =  - interrupts every 0.001 (1ms) second
    
    TCCR1B |= (1<<WGM12); // set CTC - compare timer counter with value
    TIMSK |= (1<<OCIE1A); // enable interrupt for comparing with OCR1A(H and L)
    OCR1AH = 0b00000000;  // high byte of compare value
    OCR1AL = 0b01111101;  // low byte of compare value
    TCCR1B |= (1<<CS11)|(1<<CS10);  // divider = 64
    
    TIMER = 0;
}

// TIMER1 interrupt handler
ISR (TIMER1_COMPA_vect){
    if (MODE == TIMER_MODE && !TIMER_PAUSED) {
        TIMER++;
    }
    TCOUNTER++;    
}
