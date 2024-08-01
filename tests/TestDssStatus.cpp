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

TEST(DssStatus,ICD) {
    using namespace DssApi::JsonUtils;

    DssStatus  example;

    // Setup Server status
    ServerStatus  srvr;
    srvr.setAvailability(ServerStatus::FLT_OPN,true);
    srvr.setAvailability(ServerStatus::INT_AVL,true);
    srvr.setAvailability(ServerStatus::LPNS_AVL,true);
    srvr.setUpTime(52);

    example.set(srvr);


    // Seat 25B
    SeatStatus  B25;
    B25.setStatus(SeatStatus::DSS_COMM_LOSS,true);
    B25.setStatus(SeatStatus::TM_SYNC,true);
    B25.setUIState(SeatStatus::MAP);

    example.add("25B",B25);

    // Seat 43F
    SeatStatus F43;
    F43.setStatus(SeatStatus::VLS,true);
    F43.setStatus(SeatStatus::PCTL_LOCK,true);
    F43.setMode(SeatStatus::LOGIN_AVL,true);
    F43.setMode(SeatStatus::KID,true);
    F43.setUIState(SeatStatus::PAIRING);

    example.add("43F",F43);

    // ICD: 44C80034 1910C007 2B5014A2
    std::string b64 = example.asBase64();

    ASSERT_EQ(b64,"RMgANBkQwAcrUBSi");
}