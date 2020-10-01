#include "gtest/gtest.h"

#include "team195/CKSimDriver.h"
#include <errno.h>
#include <iostream>

TEST(ErrTest, TestCompile) {
    ASSERT_EQ(c_CKSimDriver(), 0);
    c_CKSimDealloc();
}