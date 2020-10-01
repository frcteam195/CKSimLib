#include "gtest/gtest.h"

#include "team195/CKSimDriver.h"
#include <errno.h>
#include <iostream>

TEST(ZMQTest, SubRecvMsg) {
    ASSERT_EQ(c_CKSimDriver(), 0);
    //In order for this to pass, the TestZMQSrv.py server must be running
    EXPECT_GT(c_ZMQSubRecv(), 0);
    c_CKSimDealloc();
}