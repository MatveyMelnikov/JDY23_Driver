#include "jdy23_io.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal_uart.h"
#include <stdbool.h>

// extern UART_HandleTypeDef *jdy23_uart;

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
    current_io_functions.io_set_pwrc == NULL
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
  
  // return (jdy23_status)HAL_UART_Receive(
  //   jdy23_uart, data, data_size, JDY23_TIMEOUT
  // );

  return current_io_functions.io_read(data, data_size);
}

jdy23_status jdy23_io_write(
  const uint8_t *const data,
  const uint16_t data_size
)
{
  if (is_io_functions_not_initialized())
    return JDY23_ERROR;
  
  // return (jdy23_status)HAL_UART_Transmit(
  //   jdy23_uart, data, data_size, JDY23_TIMEOUT
  // );

  //current_io_functions.io_set_pwrc(false);
  jdy23_status status = current_io_functions.io_write(data, data_size);
  //current_io_functions.io_set_pwrc(true);

  return status;
}

jdy23_status jdy23_io_read_external_data(
  uint8_t *const data,
  const uint16_t data_size
)
{
  if (is_io_functions_not_initialized())
    return JDY23_ERROR;

  //return (jdy23_status)HAL_UART_Receive_DMA(jdy23_uart, data, data_size);
  return current_io_functions.io_external_read(data, data_size);
}

jdy23_status jdy23_io_set_baudrate(const uint32_t baudrate)
{
  if (is_io_functions_not_initialized())
    return JDY23_ERROR;

  return current_io_functions.io_set_baudrate(baudrate);
}
