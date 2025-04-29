#include <avr/io.h> //defines pins, ports etc
#include <util/delay.h> //functions for wasting time
#include <avr/interrupt.h>

#define PORT_SEG_A B, 5
#define PORT_SEG_B B, 4
#define PORT_SEG_C B, 3
#define PORT_SEG_D B, 2
#define PORT_SEG_E B, 1
#define PORT_SEG_F B, 0
#define PORT_SEG_G D, 7

uint8_t digit[10] = {
	/*0*/ 0x3f,
	/*1*/ 0x6,
	/*2*/ 0x5b,
	/*3*/ 0x4f,
	/*4*/ 0x66,
	/*5*/ 0x6d,
	/*6*/ 0x7d,
	/*7*/ 0x7,
	/*8*/ 0x7f,
	/*9*/ 0x6f
};

#define CMN_ANOD 0
#define CMN_KATOD 1

char common = CMN_ANOD;

#define INIT_SEG(port, pin) DDR##port |= (1<<P##port##pin)
#define INIT_SEG_EXPAND(x) INIT_SEG(x)

#define SEG_SET(port, pin, val) \
	if(val) { \
		if (CMN_ANOD == common) \
			PORT##port &= ~(1<<P##port##pin); \
		else if (CMN_KATOD == common) \
			PORT##port |= (1<<P##port##pin); \
	} else { \
		if (CMN_ANOD == common) \
			PORT##port |= (1<<P##port##pin); \
		else if (CMN_KATOD == common) \
			PORT##port &= ~(1<<P##port##pin); \
	}

#define SEG_SET_EXPAND(x, val) SEG_SET(x, val)

#define SEG_A(val) SEG_SET_EXPAND(PORT_SEG_A, val)
#define SEG_B(val) SEG_SET_EXPAND(PORT_SEG_B, val)
#define SEG_C(val) SEG_SET_EXPAND(PORT_SEG_C, val)
#define SEG_D(val) SEG_SET_EXPAND(PORT_SEG_D, val)
#define SEG_E(val) SEG_SET_EXPAND(PORT_SEG_E, val)
#define SEG_F(val) SEG_SET_EXPAND(PORT_SEG_F, val)
#define SEG_G(val) SEG_SET_EXPAND(PORT_SEG_G, val)

#define DIG_SET(port, pin, val) \
	if(val) { \
		PORT##port |= (1<<P##port##pin); \
	} else { \
		PORT##port &= ~(1<<P##port##pin); \
	}
#define DIG_1(val) DIG_SET(D, 6, val)
#define DIG_2(val) DIG_SET(D, 5, val)
#define DIG_3(val) DIG_SET(D, 4, val)
#define DIG_4(val) DIG_SET(D, 3, val)

void init_ports()
{
	INIT_SEG_EXPAND( PORT_SEG_A );
	INIT_SEG_EXPAND( PORT_SEG_B );
	INIT_SEG_EXPAND( PORT_SEG_C );
	INIT_SEG_EXPAND( PORT_SEG_D );
	INIT_SEG_EXPAND( PORT_SEG_E );
	INIT_SEG_EXPAND( PORT_SEG_F );
	INIT_SEG_EXPAND( PORT_SEG_G );
}

void set_digit(uint8_t disp_digit, uint8_t number)
{
	DIG_1(0)
	DIG_2(0)
	DIG_3(0)
	DIG_4(0)
	SEG_A(0);
	SEG_B(0);
	SEG_C(0);
	SEG_D(0);
	SEG_E(0);
	SEG_F(0);
	SEG_G(0);
	switch(disp_digit) {
	case 1:
		DIG_1(1);
		break;
	case 2:
		DIG_2(1);
		break;
	case 3:
		DIG_3(1);
		break;
	case 4:
		DIG_4(1);
		break;
	default:
		DIG_1(0)
		DIG_2(0)
		DIG_3(0)
		DIG_4(0)
		break;
	}
	SEG_A((digit[number] & 0x01));
	SEG_B((digit[number] & 0x02));
	SEG_C((digit[number] & 0x04));
	SEG_D((digit[number] & 0x08));
	SEG_E((digit[number] & 0x10));
	SEG_F((digit[number] & 0x20));
	SEG_G((digit[number] & 0x40));
}

void init_adc()
{
	ADMUX = 0 | (1<<REFS0);
	ADCSRA = 0 | (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

uint16_t get_adc_val()
{
	ADCSRA |= (1<<ADSC);
	while(1) {
		if (ADCSRA & (1<<ADIF)) {
			ADCSRA |= (1<<ADIF);
			break;
		}
	}
	return (ADCH << 8 | ADCL);
}
void num_to_digit(uint16_t reg_num, uint8_t *arr)
{
	reg_num *= 48;
	arr[0] = 0;
	arr[1] = 0;
	arr[2] = 0;
	arr[3] = 0;
	arr[0] = reg_num / 10000;
	reg_num = reg_num % 10000;
	arr[1] = reg_num / 1000;
	reg_num = reg_num % 1000;
	arr[2] = reg_num / 100;
	reg_num = reg_num % 100;
	arr[3] = reg_num / 10;
}

int main (void) {

	init_ports();
	init_adc();
	uint16_t count = 0;
	uint8_t d[4] = {0,0,0,0};
	while(1) {
		set_digit(1,d[0]);
		set_digit(2,d[1]);
		set_digit(3,d[2]);
		set_digit(4,d[3]);
		_delay_ms(1);
		count = get_adc_val();
		num_to_digit(count, d);
//		if (count >= 1000) {
//			dd++;
//			d[3]++;
//			count = 0;
//		}
//	//	if (d[3]>9)
//	//		d[3]=0;
//		if (d[3] > 9) {
//			d[3] = 0;
//			d[2]++;
//		}
//		if (d[2] > 9) {
//			d[2] = 0;
//			d[1]++;
//		}
//		if (d[1] > 9) {
//			d[1] = 0;
//			d[0]++;
//		}
//		if (d[0] > 9) {
//			d[0] = 0;
//		}
	}
	return(0); //end program. This never happens.
}
