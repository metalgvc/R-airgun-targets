//
//  COUNTER.c
//  targets
//
//  Created by metal gvc on 9/10/18.
//

int COUNTER = 0;

void init_counter(void);
int main_counter(void);
void set_prev_states(void);

int BTNPREV_STATES[6] = {0,0,0,0,0,0};

int BTN_RANDOM_UP_SECONDS[6] = {0,0,0,0,0,0};
uint32_t BTN_TIMER_START_TCOUNTER[6] = {0,0,0,0,0,0};

void init_counter(){
    set_prev_states();
    dot[0] = 0;
    dot[1] = 0;
    dot[2] = 0;
    dot[3] = 0;
}

int main_counter(){
    
    // check differences with prev state
    int i;
    for (i = 0; i < 6; i++) {
        if (BTNPREV_STATES[i] != BUTTONS_STATES[i]
            && BUTTONS_STATES[i] == 1 // count only pressed states
        ) {
            // increase affected targets counter
            COUNTER++;
            
            //
            if (BTN_RANDOM_UP_SECONDS[i] == 0) {
                // save start time point
                BTN_TIMER_START_TCOUNTER[i] = TCOUNTER;
                
                // generate random seconds to up target
                BTN_RANDOM_UP_SECONDS[i] = random_at_most(4)+2; // 2-6 sec
            }
        }
    }
    set_prev_states();
    
    if (COUNTER > 9) {
        COUNTER = 0;
        
        num4 = COUNTER;
        
        num3++;
        if (num3 > 9) {
            num2++;
            num3 = 0;
        }
        
        if (num2 > 9) {
            num1++;
            num2 = 0;
        }
    } else {
        num4 = COUNTER;
    }
    
    show_number();
    
    // handle random targets up
    // check if random seconds is reached and up target
    for (i = 0; i < 6; i++) {
        
        // wait for motors up targets for prevent double execution
        if (BTN_RANDOM_UP_SECONDS[i] == -1) {
            
            // reset states if target is already up
            if (BUTTONS_STATES[i] == 0) {
                BTN_RANDOM_UP_SECONDS[i] = 0;
            }
            
        // the seconds is reached - up target
        // .../10 - timer executes each 0.1 seconds so 1/0.1=10
        } else if (BTN_RANDOM_UP_SECONDS[i] > 0
            && ( (TCOUNTER - BTN_TIMER_START_TCOUNTER[i])/TCOUNTER_DIVIDER ) > BTN_RANDOM_UP_SECONDS[i]
        ) {
            BTN_RANDOM_UP_SECONDS[i] = -1;
            up_target(i+1);
            BTN_TIMER_START_TCOUNTER[i] = TCOUNTER;
        }
    }
    
    // off - signal to motors
    up_target_end_sig();
    
    return 1;
}

// set BTNPREV_STATES arr from curr btn states arr
void set_prev_states(){
    int i;
    for (i = 0; i < 6; i++) {
        if (BUTTONS_STATES[i] == 0 || BUTTONS_STATES[i] == 1) {
            BTNPREV_STATES[i] = BUTTONS_STATES[i];
        }
    }
}
