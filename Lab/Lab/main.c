//***************************************************************************
// Universidad del Valle de Guatemala
// Electrónica Digital 2
// Autor: Ruben Granados
// Hardware: ATMEGA328P
// Created: 28/01/2025
//***************************************************************************
// Lab 1
//***************************************************************************
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Display_7/Display_7.h"

// variables
uint8_t contador = 0;
uint8_t contador_2 = 0;
uint8_t ganador = 0;

unsigned char ButtonLastState_1 = 1;
unsigned char ButtonState_1 = 1;

uint8_t start = 0;
uint8_t numeros = 0;

unsigned char hexaDisplay[10]={0x3F, 0x18, 0x36, 0x3C, 0x19, 0x2D, 0x2F, 0x38, 0x3F, 0x3D};
int main(void)
{
	// salidas
	DDRD |= (1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);
	DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB5);
	DDRC = 0xFF;
	
	// entradas
	DDRD &= ~(1<<2);
	PORTD |= (1<<PD2); // pullup
	DDRD &= ~(1<<3);
	PORTD |= (1<<PD3);
	DDRB &= ~(1<<4);
	PORTB |= (1<<PB4);
	
	// Habilitar interrupciones PD2 y PD3
	EICRA |= (1 << ISC01) | (1 << ISC11);  // Flanco de bajada
	EIMSK |= (1 << INT0) | (1 << INT1);    // INT0 e INT1
	// Habilitar interrupción PB4
	PCICR |= (1 << PCIE0);     // Habilitar interrupciones
	PCMSK0 |= (1 << PCINT4);   // PB4
	sei();
	
    //***************************************************************************
    // LOOP
    //***************************************************************************
    while (1) 
    {
		
		if (numeros == 1){
			
			for ( int i = 5; i >= 0; i--){
				display_num(i);
				_delay_ms(1000);
			}
			start = 1;
			numeros = 0;
		}
		
		
		if ((contador <= 4)&&(ganador==0)&&(start == 1)){
			switch (contador)
			{
				case 0:
				PORTD &= ~(1<<PD4);
				PORTD &= ~(1<<PD5);
				PORTD &= ~(1<<PD6);
				PORTD &= ~(1<<PD7);
				break;
				case 1:
				PORTD |= (1<<PD4);
				break;
				case 2:
				PORTD |= (1<<PD4);
				PORTD |= (1<<PD5);
				break;
				case 3:
				PORTD |= (1<<PD4);
				PORTD |= (1<<PD5);
				PORTD |= (1<<PD6);
				break;
				case 4:
				PORTD |= (1<<PD4);
				PORTD |= (1<<PD5);
				PORTD |= (1<<PD6);
				PORTD |= (1<<PD7);
				ganador = 1;
				display_num(1);
				PORTB &= ~(1<<PB0);
				PORTB &= ~(1<<PB1);
				PORTB &= ~(1<<PB2);
				PORTB &= ~(1<<PB3);
				break;
			}
		} 
		if ((contador_2 <= 4)&&(ganador==0)&&(start ==1)){
			switch (contador_2)
			{
				case 0:
				PORTB &= ~(1<<PB0);
				PORTB &= ~(1<<PB1);
				PORTB &= ~(1<<PB2);
				PORTB &= ~(1<<PB3);
				break;
				case 1:
				PORTB |= (1<<PB0);
				break;
				case 2:
				PORTB |= (1<<PB0);
				PORTB |= (1<<PB1);
				break;
				case 3:
				PORTB |= (1<<PB0);
				PORTB |= (1<<PB1);
				PORTB |= (1<<PB2);
				break;
				case 4:
				PORTB |= (1<<PB0);
				PORTB |= (1<<PB1);
				PORTB |= (1<<PB2);
				PORTB |= (1<<PB3);
				ganador = 1;
				display_num(2);
				PORTD &= ~(1<<PD4);
				PORTD &= ~(1<<PD5);
				PORTD &= ~(1<<PD6);
				PORTD &= ~(1<<PD7);
				break;
				}
			 } 
		
	}
}
    //***************************************************************************
    // INTERRUPCIONES
    //***************************************************************************
	ISR(INT0_vect) {
		_delay_ms(5);  
		if (!(PIND & (1 << PD2))) {  // sigue en low
			uint8_t confirmacion = 0;
			
			// Verificar
			for (uint8_t i = 0; i < 20; i++) {
				if (!(PIND & (1 << PD2))) confirmacion++;
				_delay_ms(2);
			}

			// 5 lecturas confirmadas
			if (confirmacion >= 19) {
				if (start ==1){
					contador++;
				}
			}
		}
	}

	ISR(INT1_vect) {
		_delay_ms(5);
		if (!(PIND & (1 << PD3))) {  // sigue en low
			uint8_t confirmacion_2 = 0;
			
			// Verificar
			for (uint8_t i = 0; i < 20; i++) {
				if (!(PIND & (1 << PD3))) confirmacion_2++;
				_delay_ms(2);
			}

			// 5 lecturas confirmadas
			if (confirmacion_2 >= 19) {
				if (start ==1){
					contador_2++;
				}
				
			}
		}
	}

	ISR(PCINT0_vect) {
		ButtonState_1 = PINB&(1<<PINB4);

		// DEBOUNCE PARA PULSADOR 1
		if ((ButtonState_1 == 0)&&(ButtonLastState_1 == 1)){
			ButtonLastState_1 = 0;
			numeros = 1;
		}
		if (ButtonState_1 == 1){
			ButtonLastState_1 = 1;
		}
	}


