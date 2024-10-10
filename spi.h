//#define REZIM_1										//Zakomentovat = rezim 0, jinak REZIM_1 - REZIM_3
#define MASTER										//Zakomentovat = slave
//#define INTERUPT									//zakomentovat = zakaze preruseni
//#define LSB_FIRST									//zakomentovat = MSB prvni
#define SPEED_16									//zakomentovat = FCPU / 4 jinak SPEED_16 , SPEED_64 , SPEED_128
#define SPEED_X_2									//zakomentovat = v pripade polovièní rychlosti
#define CS_L PORTB &= ~(1 << PORTB4)
#define CS_H PORTB |= (1 << PORTB4)