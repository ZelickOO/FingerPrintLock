#include "zfm_20.h"
#include <stdio.h>

void pridej();
void odemkni();
void vypis(char nazev[] ,volatile uint8_t odpoved[]);


void odemkni() {
	int16_t id;
	char pomocne[5];
	_delay_ms(MS500);
	skenuj(0);
	img2tz(1 , 0);
	search();
	if(odpoved[9] == 0) {
		open = 1;
		id = odpoved[10] << 8;
		id = id + odpoved[11];
		sprintf(pomocne , "%d" , id);
		lcd_clrscr();
		lcd_puts("Vase ID je - ");
		lcd_puts(pomocne);
		lcd_gotoxy(0 , 1);
	}
	else open = 2;
}

void pridej() {
	char cislo[4];
	int16_t id = 0;
	while(1) {
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts("Novy uzivatel");
		lcd_gotoxy(0,1);
		lcd_puts("Priloz prst + OK");
		_delay_ms(MS300);
		while(1) {
			if((PIND&0b00001000) == 0) {
				skenuj(1);
				if(odpoved[9] != 0) { odpoved[9] = 0; lcd_clrscr(); lcd_puts(OPAKUJ); lcd_gotoxy(0 , 1); lcd_puts(PRILOZ); _delay_ms(MS500); continue;  }
				img2tz(1 , 1);
				if(odpoved[9] != 0) { odpoved[9] = 0; lcd_clrscr(); lcd_puts(OPAKUJ); lcd_gotoxy(0 , 1); lcd_puts(PRILOZ); _delay_ms(MS500); continue;  }
				break;
			}
			if((PIND&0b00010000) == 0) return;
			
		}
		lcd_puts("Opakuj + OK");
		_delay_ms(MS300);
		while(1) {
			if((PIND&0b00001000) == 0) {
				skenuj(1);
				if(odpoved[9] != 0) { odpoved[9] = 0; lcd_clrscr(); lcd_puts(OPAKUJ); lcd_gotoxy(0 , 1); lcd_puts(PRILOZ); _delay_ms(MS500); continue;  }
				img2tz(2 , 1);
				if(odpoved[9] != 0) { odpoved[9] = 0; lcd_clrscr(); lcd_puts(OPAKUJ); lcd_gotoxy(0 , 1); lcd_puts(PRILOZ); _delay_ms(MS500); continue;  }
				break;
			}
			if((PIND&0b00010000) == 0) return;
		}
		sablona();
		if(odpoved[9] == 0) break;
		else { lcd_clrscr(); lcd_puts(OPAKUJ); lcd_gotoxy(0 , 1); lcd_puts("celou akci"); _delay_ms(S1); continue; }
	}
	while(1) {
		lcd_clrscr();
		lcd_puts("Zadej ID");
		if((PIND&0b01000000) == 0) id++;
		if(id > 300) id = 300;
		if((PIND&0b00100000) == 0) id--;
		if(id < 0) id = 0;
		lcd_gotoxy(0 , 1);
		sprintf(cislo , "%d" , id);
		lcd_puts(cislo);
		if((PIND&0b00001000) == 0) {
			store(id);
			if(odpoved[9] == 0) return;
		}
		if(odpoved[9] != 0) { odpoved[9] = 0; lcd_clrscr(); lcd_puts(OPAKUJ); lcd_gotoxy(0 , 1);  _delay_ms(S1); continue;  }
		if((PIND&0b00010000) == 0) return;
		_delay_ms(200);
	}
}

void vypis(char nazev[] ,volatile uint8_t odpoved[]) {
	lcd_clrscr();
	lcd_gotoxy(0 , 0);
	lcd_puts(nazev);
	lcd_gotoxy(0 , 1);
	switch(odpoved[9]) {
		case 0x00: lcd_puts("OK"); break;
		case 0x01: lcd_puts("Chyba prenosu"); break;
		case 0x02: lcd_puts("Zadny prst"); break;
		case 0x03: lcd_puts("Neprovedeno"); break;
		case 0x06: lcd_puts("Zadny snimek"); break;
		case 0x07: lcd_puts("Malo bodu"); break;
		case 0x0A: lcd_puts("Chyba kombinace"); break;
		case 0x13: lcd_puts("Spatne heslo"); break;
		case 0x15: lcd_puts("Chybny snimek"); break;
		case 0x1A: lcd_puts("Spatny registr"); break;
		case 0xFF: lcd_puts("Timeout"); break;
		case 0x09: lcd_puts("Zadna shoda"); break;
		default: lcd_puts("Kravina"); break;
	}
	_delay_ms(S2);
	lcd_clrscr();
}