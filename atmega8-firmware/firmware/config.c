//
//  config.c
//  targets
//
//  Created by metal gvc on 9/19/18.
//

// debounce targets buttons time
// WARNING: used _delay_ms (NOT INTERRUPTS)
const int DEBOUNCE_BTN_TIME = 50; // ms

// delay between (after all target was down AND before up all)
int UP_ALL_TARGET_DELAY = 5000; // ms

// signal duration time for up targets motors
int UP_TARGET_SIGNAL_TIME = 300; // ms

int DISPLAY_FREQ = 5; // ms
