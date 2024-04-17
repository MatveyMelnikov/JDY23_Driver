#ifndef JDY23_DRIVER_H
#define JDY23_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "jdy23_defs.h"
#include "jdy23_io.h"

void jdy23_create(jdy23_io_functions io_functions);
void jdy23_destroy(void);
jdy23_status jdy23_check_link(void);
jdy23_status jdy23_set_baudrate(jdy23_baudrate baudrate);
jdy23_baudrate jdy23_determine_baudrate(void);
jdy23_status jdy23_set_name(const char* const name);
// hc06_status hc06_set_pin(const uint16_t pin);
jdy23_status jdy23_write(const uint8_t *const data, const uint16_t size);
jdy23_status jdy23_read(uint8_t *const data, const uint16_t size);
void jdy23_receive_complete(void);
bool jdy23_is_data_received(void);

#endif
