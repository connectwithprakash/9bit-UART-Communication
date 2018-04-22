/*
 * main.cpp
 *
 * Created: 1/25/2018 3:44:49 AM
 * Author : Prakash Chaudhary
 */
#define F_CPU           16000000UL

#include <avr/io.h>
#include <util/delay.h>

const uint32_t Baud_rate = 9600;

inline uint16_t Calculate_UBRR(const uint32_t Baud_dum) { return F_CPU / 16 / Baud_dum - 1; }
inline uint8_t _bv(const uint8_t bit) { return (1 << bit); }


void UART0_Initialize(void)
{
        uint16_t UBRR = Calculate_UBRR (Baud_rate);
        UBRR1H = (uint8_t)(UBRR >> 8);
        UBRR1L = (uint8_t)UBRR;
        UCSR1B |= (1 << RXEN1) | (1 << TXEN1) | (1 << UCSZ12);
        UCSR1C |= (1 << UCSZ10) | (1 << UCSZ11);
}

void UART0_Transmit(const uint16_t Data_dum)
{
        while( !(UCSR1A & (1<<UDRE1)) );
        if( Data_dum & 0x0100 )
                UCSR1B |= (1<<TXB81);
        else
                UCSR1B &= ~(1<<TXB81);
        UDR1 = Data_dum;
}

uint16_t UART0_Receive(void)
{
        while( !(UCSR1A & (1<<RXC1)) );
        uint8_t Higher_byte = UCSR1B;
        uint8_t Lower_byte = UDR1;
        return ( ((Higher_byte & (1<<RXB81)) << 7) | Lower_byte );
}

int main(void)
{
        UART0_Initialize();
        while (1)
        {
                UART0_Transmit(360);
                _delay_ms (50);
        }
}
