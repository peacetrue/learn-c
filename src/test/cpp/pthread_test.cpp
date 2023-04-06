#include "gtest/gtest.h"
#include <pthread_usage.h>

TEST(pthread, pthread_create_usage) {
    char *arg = (char *) "Hello, World!";
    EXPECT_EQ(arg, pthread_create_usage(arg));
}

