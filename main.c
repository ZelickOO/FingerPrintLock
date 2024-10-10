/*
 * zamek.c
 *
 * Created: 31.05.2018 18:29:21
 * Author : zelickoo
 */ 


#define F_CPU 12000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "lcd.c"
#include "ctecka.c"
#include <avr/eeprom.h>
#include "zfm_20.h"
#include "zfm_20.c"
void heslo();
void nastaveni();
void noveheslo();
void standby();
void set_ubrrl();
void set_rezim();
uint8_t hodnota_rezim;

ISR(TIMER0_OVF_vect) {
	casovac++;
	if(casovac >= 2700) standby();
}

ISR(INT0_vect) {
	GICR |= 0b00111111;
	casovac = 0;
	PORTA |= 0b00000100;
	odemkni();
	GIFR &= 0b11000000;
	GICR &= 0b11000000;
	TIMSK |= 0b00000001;
}


ISR(INT1_vect) {
	GICR |= 0b00111111;
	casovac = 0;
	PORTA |= 0b00000100;
	heslo();
	GIFR &= 0b11000000;
	GICR &= 0b11000000;
	TIMSK |= 0b00000001;
}


int main(void)
{
	TCCR0 = 0b00000101;
	TIMSK = 0b00000001;
	GICR = 0b11000000;
	MCUCR = 0b00001010;
	UCSRA |= 0b00000010;
    UCSRB = 0b00011000;				//nastaveni uart,povoleni vsech preruseni,povoleni prijimace a vysilace
	UCSRC = 0b10000110;				// -,,- zapis do ucsrc, asynchronni rezim,zadna parita, 1 stopbit, 8 dat. bitu
	UBRRH = 0b00000000;				//rychlost komunikace
	UBRRL = eeprom_read_byte((uint8_t*)4);						//rychlost komunikace
	DDRD = 0b00000000;
	PORTD = 0b01111100;
	DDRA = 0b11111111;
	PORTA |= 0b00001100;					
	lcd_init(LCD_DISP_ON);
	sei();
	lcd_gotoxy(0,0);
	lcd_puts("Ahoj");
	_delay_ms(S2);
for(uint8_t i = 0; i < 50;) {
	if((UCSRA&0b00100000) == 32) { UDR = 0xFF; i++; }
}
PORTA |= 0b01000000;

    while (1) {
		if(open == 1) { 
			hodnota_rezim = eeprom_read_byte((uint8_t*) 5);
			if(hodnota_rezim != 0) {
	open = 0;  
	lcd_puts("Odemykam"); 
	PORTA |= 0b00110000;
	PORTA &= 0b10111111;
	switch (hodnota_rezim) {
	case 1: _delay_ms(S5); break;
	case 2: _delay_ms(S10); break;
	case 3: _delay_ms(S15); break;
	case 4: _delay_ms(S20); break;
	case 5: _delay_ms(S30); break;	
	}
	PORTA &= 0b11001111;
	PORTA |= 0b01000000; 
	lcd_clrscr(); 
	}
	if(hodnota_rezim == 0) {
		open = 0;
		if((PINA&0b00010000) == 0) { lcd_puts("Odemykam"); PORTA |= 0b00110000; PORTA &= 0b10111111; _delay_ms(S2); PORTA &= 0b11011111; PORTA |= 0b01000000; lcd_clrscr(); }
			else { lcd_puts("Zamykam"); PORTA |= 0b00100000; PORTA &= 0b10101111; _delay_ms(S2); PORTA &= 0b11001111; PORTA |= 0b01000000; lcd_clrscr(); }
	}
		}
	if(open == 2) {
		lcd_clrscr();
		open = 0;
		PORTA |= 0b10000000;
		PORTA &= 0b10111111;
		vypis("Odemknuti" , odpoved);
		PORTA &= 0b01111111;
		PORTA |= 0b01000000;
		lcd_clrscr();
	}
	}
}

void heslo() {
	int8_t pin[] = { 0,0,0,0 };
	int8_t pom = 0;
	char cislo[1];
	lcd_clrscr();
	lcd_puts("Nastaveni");
	lcd_gotoxy(0,1);
	lcd_puts("Heslo? ");
	_delay_ms(500);
	while(1) {
		lcd_gotoxy((uint8_t)(pom + 7),1);
		if((PIND&0b01000000) == 0) { pin[pom]++; _delay_ms(MS200); }
		if(pin[pom] > 9) pin[pom] = 9;
		if((PIND&0b00100000) == 0) { pin[pom]--; _delay_ms(MS200); }
		if(pin[pom] < 0) pin[pom] = 0;
		sprintf(cislo , "%d" , pin[pom]);
		lcd_puts(cislo);
		if((PIND&0b00001000) == 0) { if(pom == 3) break; pom++; _delay_ms(500); }
		if((PIND&0b00010000) == 0) { lcd_clrscr(); return; }
	}
	if(pin[0] == eeprom_read_byte((uint8_t*)0) && pin[1] == eeprom_read_byte((uint8_t*)1) && pin[2] == eeprom_read_byte((uint8_t*)2) && pin[3] == eeprom_read_byte((uint8_t*)3)) nastaveni();
	else { lcd_clrscr(); lcd_puts("Chybny pin"); _delay_ms(S1); lcd_clrscr(); return; }
	lcd_clrscr();
	return;
}

void noveheslo() {
	int8_t pin[] = { 0,0,0,0 };
	int8_t pom = 0;
	char cislo[1];
	lcd_clrscr();
	lcd_puts("Nastaveni");
	lcd_gotoxy(0,1);
	lcd_puts("Nove heslo? ");
	_delay_ms(MS500);
	while(1) {
		lcd_gotoxy((uint8_t)(pom + 12),1);
		if((PIND&0b01000000) == 0) { pin[pom]++; _delay_ms(MS200); }
		if(pin[pom] > 9) pin[pom] = 9;
		if((PIND&0b00100000) == 0) { pin[pom]--; _delay_ms(MS200); }
		if(pin[pom] < 0) pin[pom] = 0;
		sprintf(cislo , "%d" , pin[pom]);
		lcd_puts(cislo);
		if((PIND&0b00001000) == 0) { if(pom == 3) break; pom++; _delay_ms(MS500); }
		if((PIND&0b00010000) == 0) { lcd_clrscr(); return; }
	}
	eeprom_write_byte((uint8_t*)0 , pin[0]);
    eeprom_write_byte((uint8_t*)1 , pin[1]);
	eeprom_write_byte((uint8_t*)2 , pin[2]);
	eeprom_write_byte((uint8_t*)3 , pin[3]);
	lcd_clrscr();
	lcd_puts("Heslo zmeneno");
	_delay_ms(S1);
	lcd_clrscr();
	return;
}

void nastaveni() {
	int8_t polozka = 0;
	while(1) {
		lcd_clrscr();
		lcd_puts("Nastaveni");
		if((PIND&0b01000000) == 0) polozka++;
		if(polozka > 99) polozka = 99;
		if((PIND&0b00100000) == 0) polozka--;
		if(polozka < 0) polozka = 0;
		lcd_gotoxy(0 , 1);
		switch(polozka) {
			case 0: lcd_puts("Novy zaznam"); break;
			case 1: lcd_puts("Zmena pin"); break;
			case 2: lcd_puts("Vymaz databazi"); break;
			case 3: lcd_puts("Precist param."); break;
			case 4: lcd_puts("Datova Rychlost"); break;
			case 5: lcd_puts("Mira shody"); break;
			case 6: lcd_puts("Velikost paketu"); break;
			case 7: lcd_puts("UBRRL"); break;
			case 8: lcd_puts("Rezim"); break;
		}
		_delay_ms(MS300);
		if((PIND&0b00010000) == 0) return;
		if((PIND&0b00001000) == 0) switch(polozka) {
			case 0: pridej(); break;
			case 1: noveheslo(); break;
			case 2: vymaz_vse(); break;
			case 3: reedpara(); break;
			case 4: setsyspara(4 , 1 , 12); break;
			case 5: setsyspara(5 , 1 , 5); break;
			case 6: setsyspara(5 , 0 , 3); break;
			case 7: set_ubrrl(); break;
			case 8: set_rezim(); break;
		}
		}		
	}		

void standby() {
	TIMSK = 0b00000000;
	casovac = 0;
	PORTA &= 0b00010000;
	_delay_ms(MS500);
	PORTA |= 0b00001000;
	_delay_ms(S1);
	for(uint8_t i = 0; i < 50;) {
		if((UCSRA&0b00100000) == 32) { UDR = 0xFF; i++; }
		}
	PORTA |= 0b01000000;
}

void set_ubrrl() {
	uint8_t pom = 0;
	char cislo[4];
	pom = eeprom_read_byte((uint8_t*) 4);
	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts("Nastaveni");
	lcd_gotoxy(0,1);
	lcd_puts("UBRRL =");
	while(1) {
		if((PIND&0b01000000) == 0) pom++;
		if((PIND&0b00100000) == 0) pom--;
		sprintf(cislo , "%d" , pom);
		lcd_gotoxy(8 , 1);
		lcd_puts("    ");
		lcd_gotoxy(8 , 1);
		lcd_puts(cislo);
		_delay_ms(MS300);
		if((PIND&0b00010000) == 0) return;
		if((PIND&0b00001000) == 0) { eeprom_write_byte((uint8_t*)4 , pom); lcd_clrscr(); lcd_puts("Ulozeno"); UBRRL = eeprom_read_byte((uint8_t*)4); _delay_ms(1000); return; }
		
	}
}


void set_rezim() {
	int8_t pom = 0;
	char cislo[4];
	pom = eeprom_read_byte((uint8_t*) 5);
	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts("Nastaveni");
	lcd_gotoxy(0,1);
	lcd_puts("Rezim ");
	while(1) {
		if((PIND&0b01000000) == 0) pom++;
		if(pom > 5) pom = 5;
		if((PIND&0b00100000) == 0) pom--;
		if(pom < 0) pom = 0;
		sprintf(cislo , "%d" , pom);
		lcd_gotoxy(6 , 1);
		lcd_puts("    ");
		lcd_gotoxy(6 , 1);
		lcd_puts(cislo);
		_delay_ms(MS300);
		if((PIND&0b00010000) == 0) return;
		if((PIND&0b00001000) == 0) { eeprom_write_byte((uint8_t*)5 , pom); lcd_clrscr(); lcd_puts("Ulozeno"); _delay_ms(S1); return; }
		
	}
}














