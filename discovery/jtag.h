#ifndef JTAG_H
#define JTAG_H

#include <stdint.h>
#include <stdbool.h>

#define JTAG_PORT_TDI GPIOB
#define JTAG_PIN_TDI GPIO0

#define JTAG_PORT_TDO GPIOB
#define JTAG_PIN_TDO GPIO1

#define JTAG_PORT_TCK GPIOB
#define JTAG_PIN_TCK GPIO2

#define JTAG_PORT_TMS GPIOB
#define JTAG_PIN_TMS GPIO3

void jtag_init(void);

void jtag_set_tdi(uint8_t value);
uint8_t jtag_get_tdo(void);
void jtag_set_tck(uint8_t value);
void jtag_set_tms(uint8_t value);

void jtag_set_frequency(uint16_t value);
void jtag_transfer(uint16_t size, uint8_t* input, uint8_t* output);
bool jtag_strobe(uint8_t pulses, bool tms, bool tdi);

#endif