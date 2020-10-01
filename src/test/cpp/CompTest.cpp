#include "gtest/gtest.h"

#include "team195/CKSimDriver.h"

TEST(ErrTest, TestCompile) {
    ASSERT_EQ(c_CKSimDriver(), 0);
    c_CKSimDealloc();
}