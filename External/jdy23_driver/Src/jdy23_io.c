#include "jdy23_io.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal_uart.h"
#include <stdbool.h>

// Static variables ----------------------------------------------------------

static jdy23_io_functions current_io_functions = { 0 };

// Static functions ----------------------------------------------------------

static bool is_io_functions_not_initialized()
{
  return (
    current_io_functions.io_read == NULL || 
    current_io_functions.io_write == NULL ||
    current_io_functions.io_external_read == NULL ||
    current_io_functions.io_set_baudrate == NULL ||
    current_io_functions.io_set_pwrc == NULL ||
    current_io_functions.io_delay == NULL
  );
}

// Implementations -----------------------------------------------------------

void jdy23_io_create(jdy23_io_functions io_functions)
{
  current_io_functions = io_functions;
}

void jdy23_io_destroy()
{
  current_io_functions.io_read = NULL;
  current_io_functions.io_write = NULL;
  current_io_functions.io_external_read = NULL;
  current_io_functions.io_set_baudrate = NULL;
}

jdy23_status jdy23_io_read(
  uint8_t *const data,
  const uint16_t data_size
)
{
  if (is_io_functions_not_initialized())
    return JDY23_ERROR;

  return current_io_functions.io_read(data, data_size);
}

jdy23_status jdy23_io_write(
  const uint8_t *const data,
  const uint16_t data_size
)
{
  if (is_io_functions_not_initialized())
    return JDY23_ERROR;

  jdy23_status status = current_io_functions.io_write(data, data_size);

  return status;
}

jdy23_status jdy23_io_read_external_data(
  uint8_t *const data,
  const uint16_t data_size
)
{
  if (is_io_functions_not_initialized())
    return JDY23_ERROR;

  return current_io_functions.io_external_read(data, data_size);
}

jdy23_status jdy23_io_set_baudrate(const uint32_t baudrate)
{
  if (is_io_functions_not_initialized())
    return JDY23_ERROR;

  return current_io_functions.io_set_baudrate(baudrate);
}

void jdy23_io_set_pwrc(const bool is_high)
{
  current_io_functions.io_set_pwrc(is_high);
}

void jdy23_io_delay(const uint32_t delay)
{
  current_io_functions.io_delay(delay);
}
