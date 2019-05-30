/*
 * project1.c
 *
 * Created: 23-May-19 10:27:31 AM
 * Author : ROHITH
 */ 

#define F_CPU 16000000UL			/* Define frequency here its 16MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>


int main()
{
	DDRB=0x00;
	DDRA=0x00;
	DDRC=0x00;
	DDRD=0x0F;
	PORTB =0x00;
	PORTA =0x00;
	PORTC =0x00;

	while(1)
	{
		if(((PINB&1<<PB0)==0)&&((PINA&1<<PA0)==0)&&((PINC&(1<<PC0))==0))
		{
			PORTD =0x00;
			PORTB =0x00;
			PORTA =0x00;
			PORTC =0x00;
		}
		else if(((PINB&1<<PB0)==1)&&((PINA&(1<<PA0))==0)&&((PINC&(1<<PC0))==0))
		{
			PORTD |= (1<<PD0);
			PORTD &= ~(1<<PD1);
			PORTD &= ~(1<<PD2);
			PORTB =0x00;
			PORTA =0x00;
			PORTC =0x00;
		}
		else if(((PINB&1<<PB0)==0)&&((PINA&(1<<PA0))==1)&&((PINC&(1<<PC0))==0))
		{
			PORTD |= (1<<PD1);
			PORTD &= ~(1<<PD0);
			PORTD &= ~(1<<PD2);
			PORTB =0x00;
			PORTA =0x00;
			PORTC =0x00;
		}
		else if(((PINB&1<<PB0)==0)&&((PINA&(1<<PA0))==0)&&((PINC&(1<<PC0))==1))
		{
			PORTD |= (1<<PD2);
			PORTD &= ~(1<<PD1);
			PORTD &= ~(1<<PD0);
			PORTB =0x00;
			PORTA =0x00;
			PORTC =0x00;
		}
		else if(((PINB&1<<PB0)==1)&&((PINA&(1<<PA0))==1)&&((PINC&(1<<PC0))==0))
		{
			PORTD |= (1<<PD0);
			PORTD |= (1<<PD1);
			PORTD &= ~(1<<PD2);
			PORTB =0x00;
			PORTA =0x00;
			PORTC =0x00;
		}
		else if(((PINB&1<<PB0)==1)&&((PINA&(1<<PA0))==0)&&((PINC&(1<<PC0))==1))
		{
			PORTD |= (1<<PD0);
			PORTD |= (1<<PD2);
			PORTD &= ~(1<<PD1);
			PORTB =0x00;
			PORTA =0x00;
			PORTC =0x00;
		}
		else if(((PINB&1<<PB0)==0)&&((PINA&(1<<PA0))==1)&&((PINC&(1<<PC0))==1))
		{
			PORTD |= (1<<PD1);
			PORTD |= (1<<PD2);
			PORTD &= ~(1<<PD0);
			PORTB =0x00;
			PORTA =0x00;
			PORTC =0x00;
		}
		else
		{
			PORTD |= (1<<PD0);
			PORTD |= (1<<PD1);
			PORTD |= (1<<PD2);
			PORTB =0x00;
			PORTA =0x00;
			PORTC =0x00;
		}
	}
}


