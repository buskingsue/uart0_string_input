#include "common/def.h"

void UART0_init(uint16_t baudrate);
void UART0_transmit(char data);
void UART0_transmit_string(const char *str);