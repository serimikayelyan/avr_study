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
	char c = 'a';
	char d = 0;
	DDRB |= (1<<PB5); //Data Direction Register B:
    	PORTB = 0b00000000 | 1 << 5; //turn on 5th LED bit/pin in PORT B (Pin13 in Arduino)
	while (1) {
		if (TIFR1 & (1 << OCF1A)) {
			if (a)
				a = 0;
			else
				a = 1;
			TCNT1 = 0;
			PORTB = PORTB ^ ((1 << 5));
			TIFR1 = TIFR1 & ((1<<OCF1A));
		}
		//if (TCNT1 >= 15625) {
		//	USART_send(c);
    		//	//PORTB = 0b00100000 | a << 5; //turn on 5th LED bit/pin in PORT B (Pin13 in Arduino)
		//	a=1;
		//}
		//if (TCNT1 < 15625) {
		//	a = 0;
		//}
//		else if (TCNT1 >= 31250 && TCNT1 < 15625) {
//    			PORTB = 0b00000000; //turn off all bits/pins on PB    
//			TCNT1 = 0;
//		}
	} //end loop
	return(0); //end program. This never happens.
}

//#include <avr/io.h>
//#include <util/delay.h>
//
//int main(void) {
//  DDRB = 0b00000001;
//
//  while (1) {
//    PORTB = 0b00000001;
//    _delay_ms(1000);
//    PORTB = 0b00000000;
//    _delay_ms(1000);
//  }
//
//  return 0;
//}
