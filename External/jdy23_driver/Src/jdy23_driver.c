#include "jdy23_driver.h"
#include "jdy23_defs.h"
#include "jdy23_io.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

static char output_buffer[JDY23_BUFFER_SIZE];
static char input_buffer[JDY23_BUFFER_SIZE];
static char *check_link_cmd = "AT+VER\r\n";
static char *baudrate_cmd = "AT+BAUD%u\r\n";
static char *determine_baudrate_cmd = "AT+BAUD\r\n";
static char *name_cmd = "AT+NAME%s\r\n";
//static char *change_name_cmd = "AT+NAME=%s\r\n";
//static char *change_pin_cmd = "AT+PSWD=\"%04u\"\r\n";
//static char *hex_symbols = "123456789A";
// static const uint32_t baudrate_int[] = {
//   1200, 2400, 4800, 9600, 19200, 38400, 
//   57600, 115200, 230400, 460800
// };
// static hc06_baudrate current_baudrate;
static volatile bool is_data_receiving = false;

// Defines -------------------------------------------------------------------

// #define GET_BAUDRATE_INT(baudrate_enum) \
//   baudrate_int[(baudrate_enum)]

#define GET_NUM_FROM_CHAR(str_ptr) \
  (*(str_ptr) - '0')

// Static functions ----------------------------------------------------------

static void set_check_cmd()
{
  strncpy(output_buffer, check_link_cmd, strlen(check_link_cmd) + 1);
}

static void set_baudrate_cmd(jdy23_baudrate baudrate)
{
  sprintf(output_buffer, baudrate_cmd, (uint16_t)baudrate);
}

static void set_determine_baudrate_cmd()
{
  strncpy(
    output_buffer,
    determine_baudrate_cmd,
    strlen(determine_baudrate_cmd) + 1
  );
}

static void set_name_cmd(const char *const name)
{
  sprintf(output_buffer, name_cmd, name);
}

// static void set_change_name_cmd(const char *const name)
// {
//   sprintf(buffer, change_name_cmd, name);
// }

// static void set_change_pin_cmd(const uint16_t pin)
// {
//   sprintf(buffer, change_pin_cmd, pin);
// }

static jdy23_status send_at_cmd(uint16_t input_data_size)
{
  uint16_t size_of_received_data = (input_data_size > JDY23_BUFFER_SIZE) ?
    JDY23_BUFFER_SIZE : input_data_size;

  jdy23_io_set_pwrc(false);

  // jdy23_status status = jdy23_io_read(
  //   (uint8_t*)buffer,
  //   size_of_received_data
  // );
  // status |= jdy23_io_write(
  //   (uint8_t*)buffer,
  //   strlen(buffer)
  // );

  // status |= jdy23_io_read(
  //   (uint8_t*)buffer,
  //   size_of_received_data
  // );

  // jdy23_status status = jdy23_io_write(
  //   (uint8_t*)buffer,
  //   strlen(buffer)
  // );
  // status |= jdy23_io_read(
  //   (uint8_t*)buffer,
  //   size_of_received_data
  // );

  jdy23_status status = jdy23_io_read_external_data(
    (uint8_t*)input_buffer,
    size_of_received_data
  );

  jdy23_io_delay(1);

  status |= jdy23_io_write(
    (uint8_t*)output_buffer,
    strlen(output_buffer)
  );

  jdy23_io_delay(100);

  jdy23_io_set_pwrc(true);

  if (jdy23_is_data_received())
    return status;
  else
    return status | JDY23_ERROR;

  // if (strstr(buffer, "OK") == NULL)
  //   return JDY23_ERROR;

  //hc06_delay(HC06_AT_DELAY);

  //return status;
}

// Implementations -----------------------------------------------------------

void jdy23_create(jdy23_io_functions io_functions)
{
  jdy23_io_create(io_functions);
}

void jdy23_destroy()
{
  jdy23_io_destroy();
}

jdy23_status jdy23_check_link()
{
  set_check_cmd();

  jdy23_status status = send_at_cmd(JDY23_AT_VER_LEN);
  if (status)
    return status;

  if (strstr(input_buffer, "+VER:JDY-23") == NULL)
    return JDY23_ERROR;
  else
    return JDY23_OK;
}

jdy23_status jdy23_set_baudrate(jdy23_baudrate baudrate)
{
  set_baudrate_cmd(baudrate);

  jdy23_status status = send_at_cmd(JDY23_OK_RESPONSE_LEN);
  if (strstr(input_buffer, "OK") == NULL)
    return status | JDY23_ERROR;
  else
    return status;
}

jdy23_baudrate jdy23_determine_baudrate(void)
{
  // +BAUD:0
  jdy23_baudrate result = JDY23_UNDEFINED;

  set_determine_baudrate_cmd();

  jdy23_status status = send_at_cmd(JDY23_BAUD_RESPONSE_LEN);
  char *field = strstr(input_buffer, "+BAUD:");

  if (status || field == NULL)
    return result;

  result = (jdy23_baudrate)GET_NUM_FROM_CHAR(field + 6);
  return (result > 7) ? JDY23_UNDEFINED : result;
}

// Param: module; Bluetooth name maximum: 24 bytes
// (User Manual of JDY-23 Slave Bluetooth Module pg. 13)
jdy23_status jdy23_set_name(const char* const name)
{
  if (strlen(name) > JDY23_MAX_NAME_LEN)
    return JDY23_ERROR;

  set_name_cmd(name);

  jdy23_status status = send_at_cmd(JDY23_OK_RESPONSE_LEN);
  if (strstr(input_buffer, "OK") == NULL)
    return status | JDY23_ERROR;
  else
    return status;
}

jdy23_status jdy23_write(const uint8_t *const data, const uint16_t size)
{
  return jdy23_io_write(data, size);
}

jdy23_status jdy23_read(uint8_t *const data, const uint16_t size)
{
  if (is_data_receiving)
    return JDY23_BUSY;

  jdy23_status status = jdy23_io_read_external_data(data, size);
  if (status)
    return status;

  is_data_receiving = true;
  return status;
}

void jdy23_receive_complete()
{
  is_data_receiving = false;
}

bool jdy23_is_data_received()
{
  return !is_data_receiving;
}
