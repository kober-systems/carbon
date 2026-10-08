#include <Arduino.h>
#include <unity.h>

#include <AbstractDatastore.h>
#include <PersistantDatastore.h>

using Result = AbstractDatastore<int>::result;

void test_capacity_and_out_of_bounds_access() {
    PersistantDatastore<int> backing(2);
    AbstractDatastore<int>& store = backing;
    int value = 123;

    TEST_ASSERT_EQUAL_UINT(2, store.size());
    TEST_ASSERT_TRUE(store.cnt() <= store.size());
    TEST_ASSERT_TRUE(store.read(2, value) == Result::out_of_bounds);
    TEST_ASSERT_EQUAL_INT(123, value);
    TEST_ASSERT_TRUE(store.write(2, 99, true) == Result::out_of_bounds);
}

void test_committed_entries_survive_reopening_store() {
    // Use known values: persistent storage may already contain data from a prior run.
    int value = 0;
    {
        PersistantDatastore<int> backing(2);
        AbstractDatastore<int>& store = backing;

        TEST_ASSERT_TRUE(store.write(0, 10, true) == Result::ok);
        TEST_ASSERT_TRUE(store.write(1, 20, true) == Result::ok);
        TEST_ASSERT_EQUAL_UINT(2, store.cnt());
        TEST_ASSERT_TRUE(store.read(0, value) == Result::ok);
        TEST_ASSERT_EQUAL_INT(10, value);
        TEST_ASSERT_TRUE(store.read(1, value) == Result::ok);
        TEST_ASSERT_EQUAL_INT(20, value);
    }

    PersistantDatastore<int> reopened(2);
    TEST_ASSERT_EQUAL_UINT(2, reopened.cnt());
    TEST_ASSERT_TRUE(reopened.read(0, value) == Result::ok);
    TEST_ASSERT_EQUAL_INT(10, value);
    TEST_ASSERT_TRUE(reopened.read(1, value) == Result::ok);
    TEST_ASSERT_EQUAL_INT(20, value);
}

void test_overwrite_keeps_count_and_other_entries() {
    PersistantDatastore<int> backing(2);
    AbstractDatastore<int>& store = backing;
    int value = 0;

    TEST_ASSERT_TRUE(store.write(0, 10, true) == Result::ok);
    TEST_ASSERT_TRUE(store.write(1, 20, true) == Result::ok);
    TEST_ASSERT_TRUE(store.write(0, 30, true) == Result::ok);
    TEST_ASSERT_EQUAL_UINT(2, store.cnt());
    TEST_ASSERT_TRUE(store.read(0, value) == Result::ok);
    TEST_ASSERT_EQUAL_INT(30, value);
    TEST_ASSERT_TRUE(store.read(1, value) == Result::ok);
    TEST_ASSERT_EQUAL_INT(20, value);
}

void test_uncommitted_write_is_visible_only_to_current_instance() {
    // Establish a known persisted value before testing an uncommitted change.
    PersistantDatastore<int> backing(2);
    TEST_ASSERT_TRUE(backing.write(0, 10, true) == Result::ok);
    TEST_ASSERT_TRUE(backing.write(0, 99) == Result::ok);
    int value = 0;
    TEST_ASSERT_TRUE(backing.read(0, value) == Result::ok);
    TEST_ASSERT_EQUAL_INT(99, value);

    PersistantDatastore<int> reopened(2);
    TEST_ASSERT_TRUE(reopened.read(0, value) == Result::ok);
    TEST_ASSERT_EQUAL_INT(10, value);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_capacity_and_out_of_bounds_access);
    RUN_TEST(test_committed_entries_survive_reopening_store);
    RUN_TEST(test_overwrite_keeps_count_and_other_entries);
    RUN_TEST(test_uncommitted_write_is_visible_only_to_current_instance);
    UNITY_END();
}

void loop() {}
