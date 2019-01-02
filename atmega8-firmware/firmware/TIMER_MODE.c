//
//  TIMER.c
//  targets
//
//  Created by metal gvc on 9/10/18.
//


void show_timer(int, int, int, int);

uint32_t _upTargetsStartTime = 0;

void init_timerm(){
    dot[0] = 0;
    dot[1] = 0;
    dot[2] = 1;
    dot[3] = 0;
}

void timer_start(){
    TIMER = 0;
    
    // display 0
    num1 = 0;
    num2 = 0;
    num3 = 0;
    num4 = 0;
    
    TIMER_STARTED = 1;
    TIMER_PAUSED = 0;
}


int main_timer(){
    
    // start timer on first target
    if (TIMER_STARTED == 0 && is_some_pressed() == 1) {
        timer_start();
        
        // pause timer and hold time on all targets is dead
    } else if (TIMER_STARTED == 1 && is_all_pressed() == 1) {
        TIMER_PAUSED = 1;
        
        if (_upTargetsStartTime == 0) {
            _upTargetsStartTime = TCOUNTER;
            
        // wait some time and up targets
        } else if (TCOUNTER > (_upTargetsStartTime + UP_ALL_TARGET_DELAY)) {
            _upTargetsStartTime = 0;
            up_all_targets();
        }
        
        // set timer again from 0 after all targets is up
    } else if (TIMER_STARTED == 1 && TIMER_PAUSED == 1 && is_all_released()) {
        TIMER_STARTED = 0;
    }
    
    if (
        TIMER >= (TCOUNTER_DIVIDER/10)
        //TIMER >= 10
    ) {
        TIMER = 0;
        
        num4++;
        
        if (num4 > 9) {
            num3++;
            num4 = 0;
        }
        
        if (num3 > 9) {
            num2++;
            num3 = 0;
        }
        
        if (num2 > 9) {
            num1++;
            num2 = 0;
        }
        
        dot[0] = 0;
        dot[1] = 0;
        dot[2] = 1;
        dot[3] = 0;
    }
    
    show_number();
    
    // off - signal to motors
    up_target_end_sig();
    
    return 1;
}
