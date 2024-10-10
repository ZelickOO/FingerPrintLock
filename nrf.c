#include "spi.c"

void send_cmd();


void send_cmd(uint8_t cmd, uint8_t registr) {
	uint8_t reply;
	cmd += registr;
    reply = spi_transfer(cmd);
	return reply;
}