/*
 * Calculator.c
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void send_10_bits(uint8_t reg_select, uint8_t read_or_write, uint8_t letter);

volatile uint8_t cnt;

int main(void)
{
	// Disable JTAG, free all of PortC
	MCUCR = (1<<JTD);    // Need 2 times in 4 clock cycles to disable JTAG
	MCUCR = (1<<JTD);
	
	// Data Direction
	DDRA = 0x0F;    // high 4 are input, low 4 are output
	DDRC = 0xFF;    // PORTC data byte for LCD
	DDRD |= (1<<DDD5) | (1<<DDD6) | (1<<DDD7);    // D7 = Enable, D6 = RS (Register Select), D5 = R/W, (Write is enable low, meaning write = low, read = high)
	
	//Port Initial conditions
	PORTC = 0x00;
	PORTD &= ~(1<<PD7);    // Enable Low
	PORTA = 0xFF;
	//PORTA = 0x01;    // 0001 will go through interrupt cycle (0001 ==> 1000 ==> 0001 ...)
	
	// Timer
	TCCR0B |= (1<<CS01);    // 8 prescale ==> 488 Hz overflow
	//TCCR0B |= (1<<CS00) | (1<<CS02);    // 1024 for testing pattern
	TIMSK0 |= (1<<TOIE0);
	
	//TCCR1B |= (1<<CS10) | (1<<CS11);    // 64 prescale
	//TIMSK1 |= (1<<TOIE1);
	sei();
	
	_delay_ms(100);
	
	send_10_bits(0,0,0x02);    // return home
	
	
	//Function SET, DL = 1 (8-bit mode), N = 2 (lines),
	// DATA-bits 5,4,3 ON
	send_10_bits(0,0,0x38);    // 0b00111000


	//DISPLAY ON
	// RS, R/W still set from before
	// D=1(display ON), C=0, B = 0 (C,B = Cursor stuff)
	send_10_bits(0,0,0x0C);    // 0x0C = 0b00001100


	// ENTRY MODE SET
	// ID = 1, S = 0
	send_10_bits(0,0,0x06);    // 0x06 = 0b00000110

	
	// rows are inputs, col = output = low 4 bits
	
	// put variables at top or bottom of main?
	// strings that will be the numbers in the calculation
	char first_string[16];
	char second_string[16];
	char * str_ptr = &first_string[0];
	
	char test_char;
	uint8_t LCD_position = 0;
	uint8_t has_equal_been_pressed = 0;
	uint8_t operation = 0;
	
	while (1)
	{
		// maybe put if (LCD_position < 16) ==> function that scans for button presses
		
		// Press 7
		if (!(PORTA & (1<<PA0)) && !(PINA & (1<<PINA4)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'7');    // print 7 on top right button
			while (!(PINA & (1<<PINA4))) 
			{
				cli();    // maybe do not need sei()/cli(), 
			}
			//sei();
			LCD_position++;
			*str_ptr = '7';
			str_ptr++;    // increment pointer address
			sei();
		}
		
		// Press 8
		if (!(PORTA & (1<<PA0)) && !(PINA & (1<<PINA5)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'8');
			while (!(PINA & (1<<PA5))) 
			{
				cli();
			}
			LCD_position++;
			*str_ptr = '8';
			str_ptr++;    // increment pointer address
			sei();
		}
		
		// Press 9
		if (!(PORTA & (1<<PA0)) && !(PINA & (1<<PINA6)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'9');
			while (!(PINA & (1<<PA6))) 
			{
				cli();
			}
			LCD_position++;
			*str_ptr = '9';
			str_ptr++;    // increment pointer address
			sei();
		}
		
		// Press 6
		if (!(PORTA & (1<<PA1)) && !(PINA & (1<<PINA6)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'6');
			while (!(PINA & (1<<PA6))) 
			{
				cli();
			}
			LCD_position++;
			*str_ptr = '6';
			str_ptr++;    // increment pointer address
			sei();
		}
		
		// Press 5
		if (!(PORTA & (1<<PA1)) && !(PINA & (1<<PINA5)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'5');
			while (!(PINA & (1<<PA5))) 
			{
				cli();    // maybe do not need sei()/cli(),
			}
			LCD_position++;
			*str_ptr = '5';
			str_ptr++;    // increment pointer address
			sei();
			_delay_ms(10);
		}
		
		// Press 4
		if (!(PORTA & (1<<PA1)) && !(PINA & (1<<PINA4)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'4');
			while (!(PINA & (1<<PA4))) 
			{
				cli();
			}
			LCD_position++;
			*str_ptr = '4';
			str_ptr++;    // increment pointer address
			sei();
		}
		
		// Press 1
		if (!(PORTA & (1<<PA2)) && !(PINA & (1<<PINA4)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'1');
			while (!(PINA & (1<<PA4))) 
			{
				cli();
			}
			LCD_position++;
			*str_ptr = '1';
			str_ptr++;    // increment pointer address
			sei();
		}
		
		// Press 2
		if (!(PORTA & (1<<PA2)) && !(PINA & (1<<PINA5)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'2');
			while (!(PINA & (1<<PA5))) 
			{
				cli();
			}
			LCD_position++;
			*str_ptr = '2';
			str_ptr++;    // increment pointer address
			sei();
			_delay_ms(10);
		}
		
		// Press 3
		if (!(PORTA & (1<<PA2)) && !(PINA & (1<<PINA6)) && LCD_position < 16 && has_equal_been_pressed == 0) 
		{
			cli();
			send_10_bits(1,0,'3');    
			while (!(PINA & (1<<PA6))) 
			{
				cli();
			}
			LCD_position++;
			*str_ptr = '3';
			str_ptr++;    // increment pointer address
			sei();
		}
		
		// Press 0
		if (!(PORTA & (1<<PA3)) && !(PINA & (1<<PINA5)) && LCD_position < 16 && has_equal_been_pressed == 0)
		{
			cli();
			send_10_bits(1,0,'0');
			while (!(PINA & (1<<PINA5)))
			{
				cli();
			}
			LCD_position++;
			*str_ptr = '0';
			str_ptr++;    // increment pointer address
			sei();
		}
		
		// Press + (top right)
		if (!(PORTA & (1<<PA0)) && !(PINA & (1<<PINA7)) && (LCD_position < 16) && (operation == 0)  && (has_equal_been_pressed == 0))
		{
			cli();
			send_10_bits(1,0,'+');
			while (!(PINA & (1<<PINA7)))
			{
				cli();
			}
			//sei();
			_delay_ms(10);    // may do not need this
			// end first string, start second string
			*str_ptr = '\0';
			str_ptr = &second_string[0];
			
			LCD_position++;
			operation = 1;
			sei();
			_delay_ms(10);
		}
		
		// Press Enter
		if (!(PORTA & (1<<PA3)) && !(PINA & (1<<PINA7))) 
		{
			while (!(PINA & (1<<PINA7))) 
			{
				cli();
			}
			//sei();
			// Do math
			_delay_ms(10);    // see if this can be deleted later
			if (operation > 0)
			{
				//set DDRAM address to 0x40, first spot on second line
				send_10_bits(0,0,0xC0);    // 0xC0 = 0b11000000
				// calculate
				//has_equal_been_pressed = 1;
				//operation = 0;
				*str_ptr = '\0';
				//try printing a string here
				
				str_ptr = &first_string[0];
				while (*str_ptr != '\0')
				{
					test_char = *str_ptr;
					str_ptr++;
					send_10_bits(1,0,test_char);    // might get messed up from uint8_t type in function
				}
				has_equal_been_pressed = 1;
				operation = 0;
				
				
			}
			// clear
			else 
			{
				// clear display
				send_10_bits(0,0,0x01);
				// return home did nothing? try set address
				send_10_bits(0,0,0x02);   // 0000001x is byte for go home
				// test for not printing after 2 enter presses, set ddram to 00
				//send_10_bits(0,0,0x80);    // 0x80 = 10000000,   set DDRAM at 0x00
				
				
				str_ptr = &first_string[0];
				LCD_position = 0;
				operation = 0;
				has_equal_been_pressed = 0;
			}
			sei();
			_delay_ms(10);    // see if can be deleted later
		}
		// Clear display go home, should be top right button
		// works when hold bottom down, click down/up another button then press up bottom left, was A3 and A4
		// has to be = then clear or run out of buttons
		
	}
}

void send_10_bits(uint8_t reg_select, uint8_t read_or_write, uint8_t letter)
{
	PORTC = 0x00;
	PORTD &= ~(1<<PD7);    // Enable Low
	//_delay_ms(50);
	if (read_or_write == 0) {
		PORTD &= ~(1<<PD5);
	}
	else {
		PORTD |= (1<<PD5);
	}
	
	if (reg_select == 1) {
		PORTD |= (1<<PD6);
	}
	else {
		PORTD &= ~(1<<PD6);
	}
	PORTC = letter;    // Data byte
	//_delay_ms(50);
	PORTD |= (1<<PD7);    // Enable High
	_delay_ms(10);
	PORTD &= ~(1<<PD7);    // Enable Low
	_delay_ms(10);
}

// Button Row Multiplexing
// 0001 ==> 0010 ==> 0100 ==> 1000 ==> 0001 ...
ISR(TIMER0_OVF_vect)
{
	if (cnt > 8) {
		cnt = 0;
		PORTA = 0xFF;
	}
	if (cnt == 1) {
		PORTA = 0xFF;
		PORTA &= ~(1<<PA0);
	}
	else if (cnt == 2) {
		PORTA = 0xFF;
	}
	else if (cnt == 3) {
		PORTA = 0xFF;
		PORTA &= ~(1<<PA1);
	}
	else if (cnt == 4) {
		PORTA = 0xFF;
	}
	else if (cnt == 5) {
		PORTA = 0xFF;
		PORTA &= ~(1<<PA2);
	}
	else if (cnt == 6) {
		PORTA = 0xFF;
	}
	else if (cnt == 7) {
		PORTA = 0xFF;
		PORTA &= ~(1<<PA3);
	}
	cnt++;
}
