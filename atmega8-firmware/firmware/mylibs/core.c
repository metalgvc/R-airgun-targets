
// check bit in var
#define GETBIT(var, pos) ( ((var) & (1<<(pos))) > 0)

void check_button(int, int);
int is_some_pressed(void);
int is_all_pressed(void);
int is_all_released(void);
void up_all_targets(void);
void up_target(int);

// targets input buttons states 1 - pressed
int BUTTONS_STATES[6] = {0,0,0,0,0,0};

uint32_t _upAllTargetLastTime = 0;
uint32_t _upOneTargetLastTime[6] = {0,0,0,0,0,0};

// read targets buttons
void check_button(btnNum, pcBit){
    
    // read btn state
    if ( GETBIT(PINC, pcBit) == 0 && BUTTONS_STATES[btnNum] == 0 ) { // pressed
        _delay_ms(DEBOUNCE_BTN_TIME);
        if (GETBIT(PINC, pcBit) == 0) { // debounce check - is still pressed ?
            BUTTONS_STATES[btnNum] = 1;
        }
    } else if ( GETBIT(PINC, pcBit) == 1 && BUTTONS_STATES[btnNum] == 1) { // released
        _delay_ms(DEBOUNCE_BTN_TIME);
        if (GETBIT(PINC, pcBit) == 1) { // is still released ?
            BUTTONS_STATES[btnNum] = 0;
        }
    }
}

// is some btn is pressed
int is_some_pressed(){
    int i;
    for (i = 0; i < 6; i++) {
        if (BUTTONS_STATES[i] == 1) {
            return 1;
        }
    }
    return 0;
}

// is all btns is pressed
int is_all_pressed(){
    int i;
    for (i = 0; i < 6; i++) {
        if (BUTTONS_STATES[i] == 0) {
            return 0;
        }
    }
    return 1;
}

// is all buttons released/ready
int is_all_released(){
    int i;
    for (i = 0; i < 6; i++) {
        if (BUTTONS_STATES[i] == 1) {
            return 0;
        }
    }
    return 1;
}

// up all targets
void up_all_targets(){
    
    // start signal
    if (PIND != 0b11111111) {
        PORTD = 0b11111111;
        _upAllTargetLastTime = TCOUNTER;
    }
}

// up specific target
void up_target(targNum){
    
    // send signall to up
    if (GETBIT(PIND, targNum-1) != 1) {
        PORTD |= 1<<(targNum-1);
        _upOneTargetLastTime[targNum-1] = TCOUNTER;
    }
}

// off - signal to motors after call up_target and up_all_targets functions
void up_target_end_sig(){
    
    int targNum;
    int _checkAllTargets = 1;
    for (targNum = 1; targNum <= 6; targNum++) {
        
        // end signall
        if (_upOneTargetLastTime[targNum-1] > 0
            && GETBIT(PIND, targNum-1) == 1
            && TCOUNTER > (_upOneTargetLastTime[targNum-1] + UP_TARGET_SIGNAL_TIME)
        ) {
            PORTD &= ~(1<<(targNum-1));
            _upOneTargetLastTime[targNum-1] = 0;
            _checkAllTargets = 0;
        }
    }
    
    if (_checkAllTargets == 1) {
        if (_upAllTargetLastTime > 0
            && PIND == 0b11111111
            && TCOUNTER > (_upAllTargetLastTime + UP_TARGET_SIGNAL_TIME)
        ) {
            PORTD = 0b00000000;
            _upAllTargetLastTime = 0;
        }
    }
}

// random number
long random_at_most(long max) {
    
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    long unsigned num_bins = (unsigned long) max + 1;
    long unsigned num_rand = (unsigned long) RAND_MAX + 1;
    long unsigned bin_size = num_rand / num_bins;
    long unsigned defect   = num_rand % num_bins;
    
    long x;
    do {
        x = rand();
    }
    // This is carefully written not to overflow
    while (num_rand - defect <= (unsigned long)x);
    
    // Truncated division is intentional
    return x/bin_size;
}
