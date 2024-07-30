//
// Created by bstil on 7/2/2024.
//

#include <gtest/gtest.h>
#include <random>
#include <format>
#include <fstream>

#include "DssCmn.h"
#include "DssStatus.h"
#include "JsonUtils.h"

using namespace DssApi;
using namespace std;

static std::default_random_engine generator;
static std::uniform_int_distribution<int> seatCount(0,330);

DssStatus loadRandomDss(int);
SeatStatus loadRandomSeat(std::string&);

TEST(DssStatus,Constructor) {
    using namespace JsonUtils;

    DssStatus tmp1;

    ASSERT_EQ(tmp1.numSeats(),0);

    for (int n=0;n<20;n++) {
        int r = seatCount(generator);

        DssStatus dss = loadRandomDss(r);
        ASSERT_EQ(dss.numSeats(), r);

        JSon obj;
        to_json(obj, dss);

        string str = to_string(obj);

        DssStatus dss2(str);
        DssStatus dss3(str);
        ASSERT_EQ(dss2, dss3);
    }

}

TEST(DssStatus,FileConstructor) {
    using namespace DssApi::JsonUtils;

    DssStatus tmp = loadRandomDss(25);

    string jsonStr = toJsonString(tmp);

}

TEST(DssStatus,Equality) {

    DssStatus tmp = loadRandomDss(seatCount(generator));
    DssStatus tmp2 = tmp;
    DssStatus tmp3;

    ASSERT_EQ(tmp,tmp2);

    ASSERT_NE(tmp,tmp3);
    ASSERT_NE(tmp2,tmp3);

    std::string id;
    tmp2.add(id,loadRandomSeat(id));
    ASSERT_NE(tmp,tmp2);
}

TEST(DssStatus,FileIO) {
    using namespace DssApi::JsonUtils;

    for (int n = 0;n<20;n++) {
        DssStatus tmp = loadRandomDss(3);
        std::string str = toJsonString(tmp);
        std::istringstream   inf(str);

        DssStatus  dssFile(inf);

        std::string a1 = toJsonString(tmp);
        std::string a2 = toJsonString(dssFile);
        ASSERT_EQ(a1,a2);
        ASSERT_EQ(dssFile,tmp);
    }
}