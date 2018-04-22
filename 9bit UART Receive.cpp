/*
 * 9bit UART Receive with Atmega2560.cpp
 *
 * Created: 2/8/2018 6:41:01 PM
 * Author : Prakash Chaudhary
 */ 


#define F_CPU           16000000UL

#include <avr/io.h>
#include <util/delay.h>

const uint32_t Baud_rate = 9600;

inline uint16_t Calculate_UBRR(const uint32_t Baud_dum) { return F_CPU / 16 / Baud_dum - 1; }

void UART0_Initialize(void)
{
	uint16_t UBRR = Calculate_UBRR (Baud_rate);
	UBRR0H = (uint8_t)(UBRR >> 8);
	UBRR0L = (uint8_t)UBRR;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << UCSZ02);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

uint16_t UART0_Receive_9bit(void)
{
	while( !(UCSR0A & (1<<RXC0)) );
	uint8_t High_byte = UCSR0B;
	uint8_t Low_byte = UDR0;
	return ( ((High_byte & (1<<RXB80)) << 7) | Low_byte );
}

int main(void)
{
	uint16_t Data = 0;
	UART0_Initialize();
	
	while (1)
	{
		Data = UART0_Receive_9bit();
		if( Data & 0x0100 )
		{DDRD |= 0xff;
		PORTD |= 0b00001111;}
		else
		{DDRD |= 0xff;
		PORTD |= 0b11110000;}
	}
}

