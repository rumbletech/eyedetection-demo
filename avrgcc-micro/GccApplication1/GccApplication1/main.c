/*
 * motor-8.c
 *
 * Created: 26/09/2019 06:25:37 ?
 * Author : shady
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


typedef enum {
	PROBLEM   = 0X00 , 
	NOPROBLEM = 0X01 
	} DRIVERSTATE ;
	
volatile DRIVERSTATE driverstate = NOPROBLEM ; 	
volatile uint8_t x = 0 ; 
	
ISR ( TIMER1_OVF_vect ){
	
	driverstate = PROBLEM ; 
	TCNT1  = 0 ;
	TCCR1B = 0 ;
	
}
	
	
ISR( USART_RXC_vect ){
	
	x= UDR ;
}
	
int main(void)
{
	
    DDRB |= ( 1 << PB1 ) ; 
	UBRRL = 51;
	UCSRB =  ( 1<<RXEN )| ( 1 << RXCIE ) ;  
	UCSRC =  (1 << UCSZ1) | (1 << UCSZ0)  | (1 << URSEL);
	TIMSK = ( 1 << TOIE1 );
	uint8_t flag = 0 ;
	sei();

	
    while (1) 
    {
		

				if ( x == 'A' && driverstate == NOPROBLEM && !flag  ){
					
									// RUN THE TIMER 
									TCCR1A = 0X00 ; 
									TCNT1  = 0 ; 
									TCCR1B = ( 0B101 << CS10 );
									flag = 1 ; 
									
									
				}
				
				else if ( x == 'B' && driverstate == NOPROBLEM ){
					
					// TIMER OFF 
					
					TCNT1  = 0 ; 
					TCCR1B = 0 ; 
					flag = 0 ;
					
				}
				
				else if ( x == 'B' && driverstate == PROBLEM ){
					
					driverstate = NOPROBLEM ; 
					PORTB &= ~ ( 1 << PB1 );     // TURN OFF MOTOR 
					flag = 0 ;
					
				}
				
				else if ( x == 'A' && driverstate == PROBLEM ){
					
					PORTB |= ( 1 << PB1 );  // TURN ON MOTOR 
				}
				
    }
}




