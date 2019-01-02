//
//  display.c
//  targets
//
//  Created by metal gvc on 9/9/18.
//

void display_init(void);
void display_show(int segmentNum, int showNumber);
void display_off(void);
void show_number(void);

uint32_t DISPL_LAST_SHOW_CNTR = 0;
int DISPL_LAST_SHOW_NUM = 1;


// display number map
int gigitsMap[11] = {
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b00111111, // 0
    0b10000000  // .
};

// display segment number map (without transistors)
//int segMap[4] = {
//    0b00001110, // 1
//    0b00001101, // 2
//    0b00001011, // 3
//    0b00000111  // 4
//};

// display segment number map (with transistors)
int segMap[4] = {
    0b00000001, // 1
    0b00000010, // 2
    0b00000100, // 3
    0b00001000  // 4
};

int num1 = 0;
int num2 = 0;
int num3 = 0;
int num4 = 0;

int dot[4] = {0,0,0,0};

// code

void display_init(){
    
}

void display_off(){
    SPI_data_start();
    SPI_send(0x00);
    SPI_send(0x00);
    SPI_data_end();
}

// show numbers on display
void show_number(){
    
    if ( (TCOUNTER - DISPL_LAST_SHOW_CNTR) > DISPLAY_FREQ /* ms */) {
        
        if (DISPL_LAST_SHOW_NUM > 4) {
            DISPL_LAST_SHOW_NUM = 1;
        }
        
        switch(DISPL_LAST_SHOW_NUM){
            case 1: display_show(1, num1); break;
            case 2: display_show(2, num2); break;
            case 3: display_show(3, num3); break;
            case 4: display_show(4, num4); break;
        }
        
        DISPL_LAST_SHOW_CNTR = TCOUNTER;
        DISPL_LAST_SHOW_NUM++;
    }
}

// show number on specific segment
void display_show(int segmentNum, int showNumber){
    
    // show number
    int digit = gigitsMap[9]; // set 0 as default
    if (showNumber > 0) {
        digit = gigitsMap[showNumber-1];
    }
    
    if (dot[segmentNum-1] == 1) {
        digit |= gigitsMap[10]; // add dot
    }
    
    //display_off();
    
    // show segment
    SPI_data_start();
    SPI_send(segMap[segmentNum-1]);
    SPI_send(digit);
    SPI_data_end();
}

void display_zero(){
    
}
