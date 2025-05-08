#include <avr/io.h> //defines pins, ports etc
#include <util/delay.h> //functions for wasting time
#include <avr/interrupt.h>

#define BAUD_RATE 9600
#define MY_UBRR F_CPU/16/BAUD_RATE-1

char surname[] = "mikayelyan";
char name[] = "sergey";

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

char USART_receive_char()
{
    while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void USART_send_int(unsigned char data)
{
	DDRB = 0 | (1<<PB5);
	unsigned char d = 0;
	if (data >= 100) {
		d = data / 100;
		USART_send(48 + d);
		data = data % 100;
	}
	if (data >= 10) {
		d = data / 10;
		USART_send(48 + d);
		data = data % 10;
	}
	if (data < 10) {
		USART_send(48 + data);
	}
	USART_send('\n');
	USART_send('\r');
}

void USART_send_string(const char *str)
{
    while (*str) {
        USART_send(*str);
        str++;
    }
    USART_send('\r');
    USART_send('\n');
}

void timer_init()
{
	TCCR1B = (1<<CS10) | (1<<CS12); //set the pre-scalar as 1024
	OCR1A = 1562;
	TCNT1 = 0;
}

int main (void) {
	USART_init();
	timer_init();
	DDRB = 0 | (1<<PB5);
	PORTB = 0 | (1<<PB5);
	char blink = 0;
	uint8_t count = 0;
	uint8_t c = 0;
	unsigned char a = 0;
	unsigned char r = 0;
	const char* st = "start";
	USART_send_string(st);
	while (1) {
		if (UCSR0A & (1 << RXC0)) {
			r = UDR0;
			switch(r) {
			case 'a':
				USART_send_string(name);
				break;
			case 'z':
				USART_send_string(surname);
				break;
			case 'l':
				PORTB |= (1<<PB5);
				blink = 0;
				break;
			case 'x':
				PORTB &= ~(1<<PB5);
				blink = 0;
				break;
			case 't':
				blink = 1;
				break;
			default:
				break;
			}
		}
			if (blink)  {
				c++;
				//USART_send_int(count);
			}
			if (c > 250) {
				count++;
				c=0;
			}
			if (count > 250) {
				count=0;
				a++;
			}
			if  (blink && a > 2)  {
				//USART_send_int(a);
				a = 0;
			       PORTB ^= (1<<PB5);
			}
		//USART_send(r);
		//USART_send('\r');
		//USART_send('\n');
		//if (r == 'a')
		//if (r =	USART_send_string(name);
		//if (r =if (r == 'z')
		//if (r =	USART_send_string(surname);
		//if (r =if (r == 'l')
		//if (r =	PORTB |= (1<<PB5);
		//if (r =if (r == 'x')
		//if (r =	PORTB &= ~(1<<PB5);


	} //end loop
	return(0); //end program. This never happens.
}
