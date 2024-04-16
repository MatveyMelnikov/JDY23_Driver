#include "unity_fixture.h"
#include "jdy23_driver.h"
#include "mock_jdy23_io.h"
#include <string.h>

// Static functions ----------------------------------------------------------

// Tests ---------------------------------------------------------------------

TEST_GROUP(jdy23_driver);

TEST_SETUP(jdy23_driver)
{
  mock_jdy23_io_create(10);
  //mock_jdy23_io_expect_baudrate_change();
  jdy23_create((jdy23_io_functions){ 0 });
}

TEST_TEAR_DOWN(jdy23_driver)
{
  //mock_jdy23_io_expect_baudrate_change();
  jdy23_destroy();
  mock_jdy23_io_verify_complete();
  mock_jdy23_io_destroy();
}

TEST(jdy23_driver, check_link_success)
{
  char* output_data = "AT+VER\r\n";
  char* input_data = "+VER:JDY-23-V1.2\r\n";

  mock_jdy23_io_expect_write((uint8_t*)output_data, strlen(output_data));
  mock_jdy23_io_expect_read_then_return(
    (uint8_t*)input_data,
    strlen(input_data)
  );

  jdy23_status status = jdy23_check_link();

  TEST_ASSERT_EQUAL(JDY23_OK, status);
}

TEST(jdy23_driver, set_baudrate_2400)
{
  char* output_data = "AT+BAUD6\r\n";
  char* input_data = "+OK";

  mock_jdy23_io_expect_write((uint8_t*)output_data, strlen(output_data));
  mock_jdy23_io_expect_read_then_return(
    (uint8_t*)input_data,
    strlen(input_data)
  );

  jdy23_status status = jdy23_set_baudrate(JDY23_2400);

  TEST_ASSERT_EQUAL(JDY23_OK, status);
}

TEST(jdy23_driver, set_baudrate_115200)
{
  char* output_data = "AT+BAUD0\r\n";
  char* input_data = "+OK";

  mock_jdy23_io_expect_write((uint8_t*)output_data, strlen(output_data));
  mock_jdy23_io_expect_read_then_return(
    (uint8_t*)input_data,
    strlen(input_data)
  );

  jdy23_status status = jdy23_set_baudrate(JDY23_115200);

  TEST_ASSERT_EQUAL(JDY23_OK, status);
}

TEST(jdy23_driver, determine_baudrate)
{
  char* output_data = "AT+BAUD\r\n";
  char* input_data = "+BAUD:3";

  mock_jdy23_io_expect_write((uint8_t*)output_data, strlen(output_data));
  mock_jdy23_io_expect_read_then_return(
    (uint8_t*)input_data,
    strlen(input_data)
  );

  jdy23_baudrate baudrate = jdy23_determine_baudrate();

  TEST_ASSERT_EQUAL(JDY23_19200, baudrate);
}
