#include "jdy23_driver.h"
#include "jdy23_defs.h"
#include "jdy23_io.h"
#include "jdy23_time.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

static char buffer[JDY23_BUFFER_SIZE];
static char *check_link_cmd = "AT+VER\r\n";
static char *baudrate_cmd = "AT+BAUD%u\r\n";
static char *determine_baudrate_cmd = "AT+BAUD\r\n";
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
  strcpy(buffer, check_link_cmd);
}

static void set_baudrate_cmd(jdy23_baudrate baudrate)
{
  sprintf(buffer, baudrate_cmd, (uint16_t)baudrate);
}

static void set_determine_baudrate_cmd()
{
  strcpy(buffer, determine_baudrate_cmd);
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
  jdy23_status status = jdy23_io_write((uint8_t*)buffer, strlen(buffer));

  uint16_t size_of_received_data = (input_data_size > JDY23_BUFFER_SIZE) ?
    JDY23_BUFFER_SIZE : input_data_size;

  status |= jdy23_io_read((uint8_t*)buffer, size_of_received_data);

  // if (strstr(buffer, "OK") == NULL)
  //   return JDY23_ERROR;

  //hc06_delay(HC06_AT_DELAY);

  return JDY23_OK;
}

// Implementations -----------------------------------------------------------

void jdy23_create(jdy23_io_functions io_functions)
{
  // hc06_io_set_baudrate(GET_BAUDRATE_INT(HC06_9600));
  // current_baudrate = HC06_9600;
  jdy23_io_create(io_functions);
}

void jdy23_destroy()
{
  // hc06_io_set_baudrate(GET_BAUDRATE_INT(HC06_9600));
  jdy23_io_destroy();
}

jdy23_status jdy23_check_link()
{
  set_check_cmd();

  jdy23_status status = send_at_cmd(JDY23_AT_VER_LEN);
  if (status)
    return status;

  if (strstr(buffer, "+VER:JDY-23") == NULL)
    return JDY23_ERROR;
  else
    return JDY23_OK;
}

jdy23_status jdy23_set_baudrate(jdy23_baudrate baudrate)
{
  set_baudrate_cmd(baudrate);

  jdy23_status status = send_at_cmd(JDY23_OK_RESPONSE_LEN);
  if (strstr(buffer, "OK") == NULL)
    return status | JDY23_ERROR;
  else
    return status;
  //status |= hc06_io_set_baudrate(GET_BAUDRATE_INT(baudrate));
  //current_baudrate = baudrate;

  return status;
}

jdy23_baudrate jdy23_determine_baudrate(void)
{
  // +BAUD:0
  jdy23_baudrate result = JDY23_UNDEFINED;

  set_determine_baudrate_cmd();

  jdy23_status status = send_at_cmd(JDY23_BAUD_RESPONSE_LEN);
  if (status || strstr(buffer, "+BAUD") == NULL)
    return result;

  result = (jdy23_baudrate)GET_NUM_FROM_CHAR(buffer + 6);

  return result;
}

// // The name is limited in 20 characters
// // (Guangzhou HC IT HC-06 product datasheet pg. 16).
// hc06_status hc06_set_name(const char* const name)
// {
//   if (strlen(name) > 20)
//     return HC06_ERROR;

//   set_change_name_cmd(name);

//   return send_at_cmd();
// }

// hc06_status hc06_set_pin(const uint16_t pin)
// {
//   if (pin > 0x270F) // 4 digits
//     return HC06_ERROR;

//   set_change_pin_cmd(pin);

//   return send_at_cmd();
// }

// hc06_status hc06_write(const uint8_t *const data, const uint16_t size)
// {
//   return hc06_io_write(data, size);
// }

// hc06_status hc06_read(uint8_t *const data, const uint16_t size)
// {
//   //return hc06_io_read(data, size);
//   if (is_data_receiving)
//     return HC06_BUSY;

//   hc06_status status = hc06_io_read_external_data(data, size);
//   if (status)
//     return status;

//   is_data_receiving = true;
//   return status;
// }

// hc06_status hc06_receive_complete(void)
// {
//   is_data_receiving = false;
// }

// bool hc06_is_data_received(void)
// {
//   return !is_data_receiving;
// }
