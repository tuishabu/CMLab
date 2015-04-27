#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
int i = 1000

void Timer_delay_ms(unsigned char d){
int j;
	for (j=0; j<(d*4); j++)
	{
	  	TCNT0 = 0x0FF;
	  	TCCR0 = 0x02;
		while((TIFR&(1<<TOV0))==0);
	  	TCCR0 = 0;
	  	TIFR = (1<<TOV0);

	}
}

ISR(INT0_vect)
{
	i=i/10;
	if (i < 1)
		i=1000;
}

int main ()
{
	GICR = (1<< INT0);
	sei();						  // Interrupt enable
	DDRC   = 0x00;                // set PORTB as input
	DDRB   = 0xFF;                // set PORTD as output
	PORTD  = 0;                   // clear PORTB

  while (1){
  	PORTB = 1<<7;
	 Timer_delay_ms(i);
	PORTB = 0;
	 Timer_delay_ms(i);
   }

}

