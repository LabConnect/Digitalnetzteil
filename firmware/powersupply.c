#include <avr/io.h>
#include <util/delay.h>
#include "powersupply.h"
#include "lcd-routines.h"
#include "rotary-encoder.c"
uint8_t val;
// Initialize SPI Master Device (without interrupt)
void spi_init_master (void)
{
    // Set MOSI, SCK as Output
    DDRB |= (1<<PB5)|(1<<PB4)|(1<<PB7);
    PORTB &= ~((1<<PB5)|(1<<PB7));
    PORTB |= (1<<PB4);

    // Enable SPI, Set as Master
    //Prescaler: Fosc/16, Enable Interrupts
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);
    _delay_ms(1);
    PORTB &= ~(1<<PB4);
    PORTB |= (1<<PB4);
}

//Function to send and receive data for both master and slave
void spi_tranceiver (unsigned char data)
{
    // Load data into the buffer
    SPDR = data;
 
    //Wait until transmission complete
    while(!(SPSR&(1<<SPIF)));
    // Return received data
}

void ADC_Init(void)
{
  ADMUX = (1<<REFS0);
  ADCSRA  = (1<<ADPS1)  | (1<<ADPS0);
  ADCSRA  |=  (1<<ADEN);
  ADCSRA  |=  (1<<ADSC);
  while (ADCSRA & (1<<ADSC))
  {   
  }
  (void) ADCW;
}

uint16_t ADC_Read(uint8_t channel)
{
  ADMUX = (ADMUX & ~(0x1f)) | (channel & 0x1F);
  ADCSRA  |=  (1<<ADSC);
  while (ADCSRA & (1<<ADSC))
  {
  }
  return ADCW*0.023972;
}

void PSU_Init(void) {
  encode_init();
	lcd_init();
	spi_init_master();
  ADC_Init();
	lcd_home();
	lcd_string("Init...");
	lcd_clear();
}

void test(void) {
	val += encode_read1();          // read a single step encoder
  char Buffer[20];
  lcd_home();
  itoa(val,Buffer,10);
  lcd_string(Buffer);
  lcd_string("  ");
  PORTB &= ~(1<<PB4);
  spi_tranceiver(0x00);
  spi_tranceiver(val);
  PORTB |= (1<<PB4);
  _delay_ms(10);
  lcd_setcursor(0,2);
  itoa(ADC_Read(6),Buffer,10);
  lcd_string(Buffer);
  lcd_string("   ");

}