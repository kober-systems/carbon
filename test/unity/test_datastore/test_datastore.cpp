#include <unity.h>

#include "../libraries/AbstractDatastore/PersistantDatastore.h"

void test_functionality_of_file1(void) {
    bool someCondition = true;
    // ... Your test code here
    TEST_ASSERT_TRUE(someCondition);
    // ... Other assertions
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_functionality_of_file1);
    UNITY_END();
}

void loop() {
    // Usually empty for unit tests
}
