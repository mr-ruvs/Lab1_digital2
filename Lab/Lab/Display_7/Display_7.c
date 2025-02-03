/*
 * Display_7.c
 *
 * Created: 29/01/2025 10:35:30
 *  Author: Ruben
 */ 
#include "Display_7.h"



void encender(void){
	PORTB |= (1<<PB5);
}

void display_num(uint8_t numero){
	unsigned char hexaDisplay[10]={0x3F, 0x18, 0x76, 0x7C, 0x59, 0x6D, 0x6F, 0x38, 0x7F, 0x7D};
		
	PORTC = hexaDisplay[numero];
	if (hexaDisplay[numero] & (1 << 6)) {
		PORTB |= (1<<PB5);
		} else {
		PORTB &= ~(1<<PB5);
	}
}