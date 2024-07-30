//
// Created by bstil on 7/2/2024.
//

#include <gtest/gtest.h>
#include <random>
#include <format>

#include "DssCmn.h"
#include "DssStatus.h"
#include "JsonUtils.h"

using namespace DssApi;
using namespace std;

static std::default_random_engine generator;
static std::uniform_int_distribution<int> seatCount(0,330);

DssStatus loadRandomDss(int);
SeatStatus loadRandomSeat();

TEST(DssStatus,Constructor) {
    using namespace JsonUtils;

    DssStatus tmp1;

    ASSERT_EQ(tmp1.numSeats(),0);

    DssStatus dss = loadRandomDss(20);
    ASSERT_EQ(dss.numSeats(),20);

    JSon obj;
    to_json(obj,dss);

    string str = to_string(obj);

    DssStatus  dss2(str);
    DssStatus  dss3(str);
    ASSERT_EQ(dss2,dss3);

    JSon obj2;
    to_json(obj2,dss2);
    string str2 = to_string(obj2);
    ASSERT_EQ(dss,dss2);
}

TEST(DssStatus,FileConstructor) {
    using namespace DssApi::JsonUtils;

    DssStatus tmp = loadRandomDss(25);

    string jsonStr = toJsonString(tmp);

    cout << jsonStr;
}

TEST(DssStatus,Equality) {

    DssStatus tmp = loadRandomDss(seatCount(generator));
    DssStatus tmp2 = tmp;
    DssStatus tmp3;

    ASSERT_EQ(tmp,tmp2);

    ASSERT_NE(tmp,tmp3);
    ASSERT_NE(tmp2,tmp3);

    tmp2.add(loadRandomSeat());
    ASSERT_NE(tmp,tmp2);
}