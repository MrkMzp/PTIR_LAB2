#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned long currentMillis = 0;
volatile uint8_t dig[3] = {0 , 0 , 0};

void DrawDigit(uint8_t digit)
{
	if(digit == 0)
	{
		PORTD &= 0xD0;
		PORTC &=  0x34;
	}
	if(digit == 1)
	{
		PORTD &= 0x80;
		PORTC &=  0x04;
	}
	if(digit == 2)
	{
		PORTD &= 0xB0;
		PORTC &=  0x30;
	}
	if(digit == 3)
	{
		PORTD &= 0xB0;
		PORTC &=  0x14;
	}
	if(digit == 4)
	{
		PORTD &= 0xC0;
		PORTC &=  0x14;
	}
	if(digit == 5)
	{
		PORTD &= 0x70;
		PORTC &=  0x14;
	}
	if(digit == 6)
	{
		PORTD &= 0x70;
		PORTC &=  0x34;
	}
	if(digit == 7)
	{
		PORTD &= 0xA0;
		PORTC &=  0x04;
	}
	if(digit == 8)
	{
		PORTD &= 0xF0;
		PORTC &=  0x34;
	}
	if(digit == 9)
	{
		PORTD &= 0xF0;
		PORTC &=  0x14;
	}
	return;
}

void ClearDisplay()
{
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	return;
}

void ShowSeconds()
{
	//first digit (x 0 0)
	PORTB = 0x04;
	DrawDigit(dig[2]);
	ClearDisplay();
	
	//second (0 x 0)
	PORTD = 0x02;
	DrawDigit(dig[1]);
	ClearDisplay();
	
	//third (0 0 x)
	PORTB = 0x02;
	DrawDigit(dig[0]);
	ClearDisplay();
}

void timer1_init() 
{
	TCCR1B |= (1 << WGM12);
	OCR1A = 125;  // For 1 ms (16 MHz CPU)
	TIMSK1 |= (1 << OCIE1A);
	TCNT1 = 0;
}

ISR(TIMER1_COMPA_vect) 
{ 
	currentMillis++;
}



int main()
{
	timer1_init();
    sei();
    
    uint8_t seconds = 0;
	
	DDRB = 0x03;
	PORTB = 0x00;
	DDRD = 0xF4;
	PORTD = 0x00;
	DDRC = 0x3C;
	PORTC = 0x00;

    while (1) 
    {
		if(currentMillis >= 999000)currentMillis = 0;
		
		seconds = currentMillis/1000;
		
		dig[0] = seconds % 10;
		dig[1] = (seconds / 10) % 10;
		dig[2] = (seconds / 100) % 10; 
		
		showSeconds();
    }
    	
	return 0;
}
