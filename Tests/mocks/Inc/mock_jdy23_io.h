#ifndef MOCK_JDY23_IO_H
#define MOCK_JDY23_IO_H

#include "jdy23_io.h"

void mock_jdy23_io_create(const uint8_t max_expectations);
void mock_jdy23_io_destroy(void);
void mock_jdy23_io_expect_write(
  const uint8_t *const data,
  const uint16_t data_size
);
void mock_jdy23_io_expect_read_then_return(
  const uint8_t *const data,
  const uint16_t data_size
);
void mock_jdy23_io_expect_read_external_then_return(
  const uint8_t *const data,
  const uint16_t data_size
);
void mock_jdy23_io_expect_baudrate_change(void);
void mock_jdy23_io_verify_complete(void);

#endif
