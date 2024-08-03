//
// Created by bstil on 7/1/2024.
//

#include <gtest/gtest.h>
#include <random>
#include <format>

#include "SeatStatus.h"
#include "JsonUtils.h"
#include "SeatID.h"

using namespace DssApi;

SeatStatus loadRandomSeat(std::string& id);

static const
std::vector<SeatStatus::Availability>  AVL = {{
    SeatStatus::DSS_COMM_LOSS,SeatStatus::TV_SVC_AVL,SeatStatus::TM_SYNC,
    SeatStatus::PA,SeatStatus::VLS,SeatStatus::PCTL_LOCK,
    SeatStatus::STOWD}};

TEST(SeatStatus, Constructors) {

    SeatStatus tmp;

    for (auto item: AVL) {
        ASSERT_EQ(tmp.getStatus(item),0);
    }

    //ASSERT_EQ(tmp.getMode(),0);
    ASSERT_EQ(tmp.getUIState(),SeatStatus::UNSPECIFIED);
}


TEST(SeatStatus, GetSetMethods) {


    const int N = sizeof(AVL)/sizeof(AVL[0]);

    SeatStatus   tmp;

    // should all be 0 after constructions
    for (int n=0;n < N;n++) {
        ASSERT_EQ(tmp.getStatus(AVL[n]),0);
    }

    for (int x=0;x<N;x++) {
        tmp.setStatus(AVL[x],true);
        for (int y=0;y<N;y++) {
            if (x == y) {
                ASSERT_EQ(tmp.getStatus(AVL[y]),true);
            } else {
                ASSERT_EQ(tmp.getStatus(AVL[y]),false);
            }
        }
        tmp.setStatus(AVL[x],false);
        ASSERT_EQ(tmp.getStatus(AVL[x]),false);

    }
}

TEST(SeatStatus, SeatIdentifiers) {

    SeatID   invalid;
    SeatID   firstClass("4B");
    SeatID   comfPlus("14D");
    SeatID   main("22F");


    // Attempt a few BAD examples
    EXPECT_THROW({SeatID a("0A");},std::runtime_error);
    EXPECT_THROW({SeatID b("A1");},std::runtime_error);
    EXPECT_THROW({SeatID c("99A");},std::runtime_error);
    EXPECT_THROW({SeatID d("32Q");},std::runtime_error);

    // Try the extremes and one in the middle
    SeatID aa("1A");
    ASSERT_EQ(1,aa.getRow());
    ASSERT_EQ('A',aa.getColumn());
    ASSERT_EQ(static_cast<std::string>(aa),"1A");

    SeatID bb("13F");
    ASSERT_EQ(13,bb.getRow());
    ASSERT_EQ('F',bb.getColumn());
    ASSERT_EQ(static_cast<std::string>(bb),"13F");

    SeatID cc("63P");
    ASSERT_EQ(63,cc.getRow());
    ASSERT_EQ('P',cc.getColumn());
    ASSERT_EQ(static_cast<std::string>(cc),"63P");

}

#if 0
// TODO remove seatIDs
TEST(SeatStatus,Equality) {
    SeatStatus  tmp("1A");
    SeatStatus  tmp2("1A");
    SeatStatus  tmp3("24A");

    ASSERT_EQ(tmp,tmp2);
    tmp.setStatus(DssApi::SeatStatus::DSS_COMM_LOSS, 1);
    ASSERT_NE(tmp,tmp2);
    tmp.setStatus(DssApi::SeatStatus::DSS_COMM_LOSS, 0);
    ASSERT_EQ(tmp,tmp2);

    ASSERT_NE(tmp,tmp3);
}
#endif

TEST(SeatStatus,ReadWriteBinary) {
    std::string id;
    SeatStatus tmp = loadRandomSeat(id);

    tmp.setStatus(DssApi::SeatStatus::TV_SVC_AVL, 1);
    tmp.setMode(DssApi::SeatStatus::Mode::KID,1);
    tmp.setUIState(DssApi::SeatStatus::MAP);

    ByteBuffer buffer;
    tmp.write(buffer);

    SeatStatus  tmp2;
    std::string id2;
    ByteBuffer::const_iterator it = buffer.cbegin();
    tmp2.read(it);

    ASSERT_EQ(tmp,tmp2);
//    ASSERT_EQ(id,id2);
}

TEST(SeatStatus,ReadWriteJSON) {
    using namespace DssApi::JsonUtils;
    using namespace std;

    for (int n=0;n<100;n++) {
        std::string id;
        SeatStatus tmp = loadRandomSeat(id);
        SeatStatus tmp2;

        ASSERT_NE(tmp,tmp2);

        JSon   obj;
        to_json(obj,tmp);

        auto ans = toJsonString(tmp);

        from_json(obj,tmp2);
        ASSERT_EQ(tmp,tmp2);

        auto str = toJsonString(tmp2);
        SeatStatus tmp3;
        ASSERT_NE(tmp,tmp3);

        // from a string
        SeatStatus tmp4;
        fromJsonString(str,tmp4);
        ASSERT_EQ(tmp2,tmp4);

    }
}