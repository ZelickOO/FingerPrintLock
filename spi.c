#include "spi.h"
void spi_on();
void spi_init();
void spi_transfer(uint8_t data);

void spi_init() {
	#ifdef MASTER
	SPCR |= (1 << MSTR);
	DDRB |= (1 << DDB3) | (1 << DDB4) | (1 << DDB5) | (1 << DDB7);
	CS_H;
	#else
	DDRB |= (1 << DDB6);	
	#endif
	
	#ifdef INTERUPT
	SPCR |= (1 << SPIE);
	#endif
	
	#ifdef LSB_FIRST
    SPCR |= (1 << DORD);
	#endif
	
	#ifdef REZIM_1
	SPCR |= (1 << CPHA);	
	#endif

	#ifdef REZIM_2
	SPCR |= (1 << CPOL);
	#endif
	
	#ifdef REZIM_3
	 SPCR |= (1 << CPOL);
	 SPCR |= (1 << CPHA);
	#endif
	
	#ifdef SPEED_16
	SPCR |= (1 << SPR0); 
	#endif
	
	#ifdef SPEED_64
	SPCR |= (1 << SPR1);
	#endif
	
	#ifdef SPEED_128
	SPCR |= (1 << SPR0);
	SPCR |= (1 << SPR1); 
    #endif
	
	#ifdef SPEED_X_2
	SPSR |= (1 << SPI2X);
	#endif
	
}

void spi_on() {
	CS_H;
	SPCR |= (1 << SPE);
}

void spi_transfer() {
	CS_L;
	SPDR = data;
	while(SPSR&(1 << SPIF) == 0) {}
		CS_H;
		return SPDR;
}