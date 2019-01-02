//
//  SPI.c
//  targets
//
//  Created by metal gvc on 9/9/18.
//

// SPI pins of PORTB
#define MOSI 3
#define MISO 4
#define SCK 5
#define SS 2

// SPI registers
#define SPE 6 
#define MSTR 4
#define SPR0 0
#define SPIF 7
#define DORD 5

void SPI_init(void);
void SPI_send(int data);
void SPI_data_start(void);
void SPI_data_end(void);

// code

// init SPI
void SPI_init(){
    DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS); // set MOSI, SCK, SS as output
    DDRB &= ~(1<<MISO);                 // set MISO as input
    
    SPCR = 0b00000000;
    SPCR |= (1<<SPE)|(1<<MSTR);          // SPI is on & set as master
    //SPCR = 0b11010000;                  // http://samou4ka.net/page/interfejs-spi-mikrokontrollerov-avr
    
    //PORTB |= (1<<SS);                   // set SPI for send data;
    PORTB &= ~(1<<SS);
     
}

void SPI_data_start(){
    //PORTB &= ~(1<<SS); // SS -> 0
    PORTB |= (1<<SS);  // SS -> 1
}

void SPI_data_end(){
    //PORTB |= (1<<SS);  // SS -> 1
    PORTB &= ~(1<<SS); // SS -> 0
}

// send byte via SPI
void SPI_send(int data){
    SPDR = data;                    // send data
    while(!(SPSR & (1<<SPIF)));     // wait untill sending    
}
