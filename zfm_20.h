#define ADRESA 0xFFFFFFFF
#define HLAVICKA 0xEF01
#define OK "Dokonceno"
#define TRANSFERERR "Chyba prenosu"
#define FAILED "Chyba operace"
#define OPAKUJ "Chyba - opakuj"
#define PRILOZ "Priloz prst + OK"
#define MS200 200
#define MS300 300
#define MS500 500
#define S1 1000
#define S2 2000
#define S5 5000
#define S10 10000
#define S15 15000
#define S20 20000
#define S30 30000

volatile uint8_t odpoved[30];
volatile uint8_t open;
volatile uint16_t casovac;

void odesli(uint8_t prikaz[] , uint8_t pocet);
void vymaz_vse();
void inicializace();
void img2tz(uint8_t buffer , uint8_t info);
void setsyspara(uint8_t parametr , uint8_t min, uint8_t max);
void reedpara();
void werifypasswd();
void prijmy(uint8_t pocet);
void setpasswd();
void sablona();
void skenuj(uint8_t info);
void store();
void search();

