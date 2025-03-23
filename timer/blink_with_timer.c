#include <avr/io.h> //defines pins, ports etc
#include <util/delay.h> //functions for wasting time

#define BAUD_RATE 9600
#define MY_UBRR F_CPU/16/BAUD_RATE-1

void USART_init(void) {
    unsigned int ubrr = MY_UBRR;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)(ubrr);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void USART_send(char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void USART_send_string(const char *str)
{
    while (*str) {
        USART_send(*str);
        str++;
    }
}

void timer_init()
{
	TCCR1B = (1<<CS10) | (1<<CS12); //set the pre-scalar as 1024
	OCR1A = 15625;
	TCNT1 = 0;
}

void send_int()
{
	
}
int main (void) {
//init
	timer_init();
//	USART_init();
	char a = 0;

  	// Set PD0 and PD1 as outputs (pinMode function handles this)
	DDRD = 1 << PD1; //Data Direction Register D:
	DDRB = (1<<PB5); //Data Direction Register D:
	PORTD = 1 << PD1;  // Set PD0 HIGH (turn on LED connected to pin 0)
	PORTB = (1 << PB5); // Set PD1 LOW (turn off LED connected to pin 1)
	while (1) {
		if (TIFR1 & (1 << OCF1A)) {
			TCNT1 = 0;
			if (a>=2) {
				PORTD = PORTD ^ (1 << PD1);
				a = 0;
			}
			PORTB = PORTB ^ ((1 << PB5));
			TIFR1 = TIFR1 & ((1<<OCF1A));
			a++;
		}
	} //end loop
	return(0); //end program. This never happens.
}
