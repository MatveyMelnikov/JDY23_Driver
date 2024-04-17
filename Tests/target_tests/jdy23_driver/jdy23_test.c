#include "unity_fixture.h"
#include "jdy23_driver.h"
#include "jdy23_io.h"

// Static functions ----------------------------------------------------------

// Tests ---------------------------------------------------------------------

TEST_GROUP(jdy23_driver);

TEST_SETUP(jdy23_driver)
{
  //jdy23_create();
}

TEST_TEAR_DOWN(jdy23_driver)
{
  //hc06_set_baudrate(HC06_9600);
  //jdy23_destroy();
  jdy23_set_baudrate(JDY23_9600);
}

TEST(jdy23_driver, check_link_success)
{
  jdy23_status status = jdy23_check_link();
  status |= jdy23_check_link();

  TEST_ASSERT_EQUAL(JDY23_OK, status);
}

TEST(jdy23_driver, set_baud_rate_9600)
{
  jdy23_status status = jdy23_set_baudrate(JDY23_9600);
  jdy23_baudrate baudrate = jdy23_determine_baudrate();

  TEST_ASSERT_EQUAL(JDY23_OK, status);
  TEST_ASSERT_EQUAL(JDY23_9600, baudrate);
}

TEST(jdy23_driver, set_baud_rate_2400)
{
  jdy23_status status = jdy23_set_baudrate(JDY23_2400);
  jdy23_baudrate baudrate = jdy23_determine_baudrate();

  TEST_ASSERT_EQUAL(JDY23_OK, status);
  TEST_ASSERT_EQUAL(JDY23_2400, baudrate);
}

TEST(jdy23_driver, set_name)
{
  jdy23_status status = jdy23_set_name("jdy-23-test");

  TEST_ASSERT_EQUAL(JDY23_OK, status);
}
