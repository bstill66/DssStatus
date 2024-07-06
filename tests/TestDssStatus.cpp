//
// Created by bstil on 7/2/2024.
//

#include <gtest/gtest.h>
#include <random>
#include <format>

#include "DssStatus.h"
#include "JsonUtils.h"

using namespace DssApi;

TEST(DssStatus,Constructor) {
    DssStatus  tmp;

    ASSERT_EQ(tmp.numSeats(),0);
}