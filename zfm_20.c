
#include <stdio.h>
#include "zfm_20.h"




void search() {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0x00, 0x08 , 0x04 , 0x01 , 0x00 , 0x00 , 0x01 , 0x2C , 0x00 , 0x3B } ;
	odesli(prikaz , 17);
	prijmy(16);
	return;
}

void store(uint16_t id) {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0x00, 0x06 , 0x06 , 0x01 , 0x00 , 0x00 , 0x00 , 0x0E} ;
	prikaz[11] = (uint8_t)(id >> 8);
	prikaz[12] = (uint8_t)id;
	id = id + 14;
	prikaz[14] = (uint8_t)id;
	if(id > 255) prikaz[14] += 1;
	odesli(prikaz , 15);
	prijmy(12);
	vypis("Ulozeni" , odpoved);
	return;
}



void sablona() {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0x00, 0x03 , 0x05 , 0x00 , 0x09} ;
	odesli(prikaz , 12);
	prijmy(12);
	vypis("Sablona" , odpoved);
	return;
}


void img2tz(uint8_t buffer , uint8_t info) {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0x00, 0x04 , 0x02 , 0x01 , 0x00 , 0x08} ;
	prikaz[10] = buffer;
	prikaz[12] += (buffer - 1);
	odesli(prikaz , 13);
	prijmy(12);
	if(info == 1) vypis("Charakteristika" , odpoved);
	return;
}

void setsyspara(uint8_t parametr , uint8_t min, uint8_t max) {
	_delay_ms(MS300);
	int8_t hodnota = 0;
	char pomocne[3];
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0x00, 0x05 , 0x0E , 0x00 , 0x00 , 0x00 , 0x14} ;
	prikaz[10] = parametr;
	while(1) {
		if((PIND&0b01000000) == 0) hodnota++;
		if(hodnota > max) hodnota = max;
		if((PIND&0b00100000) == 0) hodnota--;
		if(hodnota < min) hodnota = min;
		prikaz[11] = hodnota;
		if((PIND&0b00010000) == 0) return;
		if((PIND&0b00001000) == 0) break;
		sprintf(pomocne , "%d" , hodnota);
		lcd_clrscr();
		switch(parametr) {
			case 4: lcd_puts("Datova rychlost"); break;
			case 5: lcd_puts("Mira shody"); break;
			case 6: lcd_puts("Velikost paketu"); break;
		}
		lcd_gotoxy(0 , 1);
		lcd_puts(pomocne);
		_delay_ms(MS300);
	}
	prikaz[13] = prikaz[13] + parametr + hodnota;
	odesli(prikaz , 14);
	prijmy(12);
	vypis("Nastaveni" , odpoved);
	return;
}

void setpasswd() {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0x00, 0x07 , 0x12 , 0 , 0 , 0 , 1 , 0x00 , 0x1B} ;
	odesli(prikaz , 16);
	prijmy(12);
	vypis("Set passwd" , odpoved);
	return;
}

void reedpara() {
	char pomocne[5];
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0x00, 0x03 , 0x0F , 0x00 , 0x13 } ;
	odesli(prikaz , 12);
	prijmy(28);
	vypis("Akt. nastaveni" , odpoved);
	for(int8_t i = 10; i != 26; i++) {
		sprintf(pomocne , "%X" , odpoved[i]);
		lcd_clrscr();
		_delay_ms(MS500);
		lcd_puts(pomocne);
		_delay_ms(S1);
	}
	return;
}

void inicializace() {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0x00, 0x04 , 0x17 , 0x00 , 0x00 , 0x1C} ;
	odesli(prikaz , 13);
	prijmy(12);
	vypis("Inicializace" , odpoved);
	return;
}

void vymaz_vse() {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0 , 0x03 , 0x0D , 0 , 0x11} ;
	odesli(prikaz , 12);
	prijmy(12);
	vypis("Vymazani vsech" , odpoved);
	return;
}

void skenuj(uint8_t info) {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA , 0x01 , 0 , 0x03 , 0x01 , 0 , 0x05} ;
	odesli(prikaz , 12);
	prijmy(12);
	if(info == 1) vypis("Skenovani" , odpoved);
	return;
}

void werifypasswd() {
	uint8_t prikaz[] = {(uint8_t)(HLAVICKA >> 8) , (uint8_t)HLAVICKA , (uint8_t)(ADRESA >> 24) , (uint8_t)(ADRESA >> 16) , (uint8_t)(ADRESA >> 8) , (uint8_t)ADRESA ,0x01 , 0x00 , 0x07, 0x13 , 0 , 0 , 0 , 0 ,0x00 , 0x1B} ;
	odesli(prikaz , 16);
	prijmy(12);
	vypis("Overeni hesla" , odpoved);
	return;
}
void odesli(uint8_t prikaz[] , uint8_t pocet) {
	uint8_t pomocna;
	pomocna = 0;
	while(1) {
		if((UCSRA&0b00100000) == 32) {UDR = prikaz[pomocna]; pomocna++;}
		if(pomocna == (pocet)) break;
	}
	return;
}

void prijmy(uint8_t pocet) {
	uint8_t pomocna = 0;
	uint16_t timer = 0;
	while(1) {
		while((UCSRA&0b10000000) == 128) {
			odpoved[pomocna] = UDR;
			timer++;
			if(timer >= 20000) break;
			if(pomocna == 0 && odpoved[0] != 0xEF) continue;
			pomocna++;
		}
		timer++;
		if(pomocna >= pocet) break;
		if(timer >= 20000) { odpoved[9] = 0xFF; break; }
		if((UCSRA&0b10000000) != 128) _delay_us(100);
	}
	return;
}















