#include "../librobot.c"
#include "../unity/src/unity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_Basics()
{
  TEST_ASSERT(true);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_Basics);
  return UNITY_END();
}