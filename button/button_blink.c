#include <avr/io.h> //defines pins, ports etc
#include <util/delay.h> //functions for wasting time
#include <avr/interrupt.h>

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

//void USART_send_string(const char *str)
//{
//    while (*str) {
//        USART_send(*str);
//        str++;
//    }
//}

//void timer_init()
//{
//	TCCR1B = (1<<CS10) | (1<<CS12); //set the pre-scalar as 1024
//	OCR1A = 1562;
//	TCNT1 = 0;
//	TIMSK1 |= 1 << OCIE1A;
//}

//volatile uint8_t tick1_signal = 0;
//volatile uint8_t tick2_signal = 0;

//ISR(TIMER1_COMPA_vect, ISR_BLOCK)
//{
//	tick1_signal++;
//	tick2_signal++;
//	if (tick1_signal == 9) {
//		PORTB = PORTB | ((1 << PB5));
//	} else 
//		PORTB = PORTB & ~((1 << PB5));
//	if (tick1_signal >= 10)
//		tick1_signal = 0;
//
//	if (tick2_signal == 19) {
//		PORTD = PORTD & ~((1 << PD1));
//	} else 
//		PORTD = PORTD | ((1 << PD1));
//	if (tick2_signal >= 20)
//		tick2_signal = 0;
//	TCNT1 = 0;
//         // Timer1 CTC flag auto-cleared by hardware
//}

int main (void) {
	//timer_init();
	USART_init();
	char a = 0;
	char pressed1 = 0;
	char pressed2 = 0;
	char oldstat = 0;

	DDRB = 0 | (1<<PB5) | (1<<PB4) | (1<<PB3);
	//PORTB = (1 << PB5) | (1<<PB3) | (1<<PB2);
	PORTB = (1<<PB2) | (1<<PB1);
	while (1) {
		if (pressed1 == 0) {
			if (!(PINB & (1<<PB2))) {
				pressed1 = 1;
			}
		}
		if (pressed1 == 1) {
			if (PINB & (1<<PB2)) {
				PORTB ^= (1<<PB3);
				pressed1 = 0;
			}
		}
		if (pressed2 == 0) {
			if (!(PINB & (1<<PB1))) {
				pressed2 = 1;
			}
		}
		if (pressed2 == 1) {
			if (PINB & (1<<PB1)) {
				PORTB ^= (1<<PB5);
				pressed2 = 0;
			}
		}
	} //end loop
	return(0); //end program. This never happens.
}
