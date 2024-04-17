#include "unity_fixture.h"

TEST_GROUP_RUNNER(jdy23_driver)
{
  RUN_TEST_CASE(jdy23_driver, send_success);
  RUN_TEST_CASE(jdy23_driver, check_link_success);
  RUN_TEST_CASE(jdy23_driver, set_baudrate_2400);
  RUN_TEST_CASE(jdy23_driver, set_baudrate_115200);
  RUN_TEST_CASE(jdy23_driver, determine_baudrate);
  RUN_TEST_CASE(jdy23_driver, set_name_success);
  RUN_TEST_CASE(jdy23_driver, set_name_fail);
  RUN_TEST_CASE(jdy23_driver, external_read_success);
}