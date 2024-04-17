#ifndef JDY23_IO_H
#define JDY23_IO_H

#include <stdint.h>
#include <stdbool.h>
#include "jdy23_defs.h"

typedef struct {
  jdy23_status (*io_read)(uint8_t *const, const uint16_t);
  jdy23_status (*io_external_read)(uint8_t *const, const uint16_t);
  jdy23_status (*io_write)(const uint8_t *const, const uint16_t);
  jdy23_status (*io_set_baudrate)(const uint32_t);
  void (*io_set_pwrc)(const bool);
  void (*io_delay)(const uint32_t);
} jdy23_io_functions;

void jdy23_io_create(jdy23_io_functions io_functions);
void jdy23_io_destroy(void);
jdy23_status jdy23_io_read(
  uint8_t *const data,
  const uint16_t data_size
);
jdy23_status jdy23_io_write(
  const uint8_t *const data,
  const uint16_t data_size
);
jdy23_status jdy23_io_read_external_data(
  uint8_t *const data,
  const uint16_t data_size
);
jdy23_status jdy23_io_set_baudrate(const uint32_t baudrate);
void jdy23_io_set_pwrc(const bool is_high);
void jdy23_io_delay(const uint32_t delay);

#endif
