//
// Created by bstil on 7/1/2024.
//

#include <gtest/gtest.h>
#include <random>
#include <format>

#include "SeatStatus.h"
#include "JsonUtils.h"

using namespace DssApi;

SeatStatus loadRandomSeat(std::string& id);

TEST(SeatStatus, Constructors) {

    EXPECT_NO_THROW({SeatStatus tmp("1A");});
    EXPECT_NO_THROW({SeatStatus tmp("63P");});
    EXPECT_NO_THROW({SeatStatus tmp("27F");});

    EXPECT_THROW({SeatStatus  tmp("A1");},std::runtime_error);
    EXPECT_THROW({SeatStatus  tmp("32FF");},std::runtime_error);
}

TEST(SeatStatus, GetSetMethods) {
    SeatStatus   tmp;


}

TEST(SeatStatus, SeatIdentifiers) {
    SeatStatus   tmp;

    // Attempt a few BAD examples
    EXPECT_THROW({tmp.setSeatId("0A");},std::runtime_error);
    EXPECT_THROW({tmp.setSeatId("A1");},std::runtime_error);
    EXPECT_THROW({tmp.setSeatId("99A");},std::runtime_error);
    EXPECT_THROW({tmp.setSeatId("32Q");},std::runtime_error);

    // Try the extremes and one in the middle
    ASSERT_NO_THROW({tmp.setSeatId("1A");});
    ASSERT_EQ(1,tmp.getRow());
    ASSERT_EQ('A',tmp.getColumn());
    ASSERT_EQ("1A",tmp.getSeatId());

    ASSERT_NO_THROW({tmp.setSeatId("13F");});
    ASSERT_EQ(13,tmp.getRow());
    ASSERT_EQ('F',tmp.getColumn());
    ASSERT_EQ("13F",tmp.getSeatId());

    ASSERT_NO_THROW({tmp.setSeatId("63P");});
    ASSERT_EQ(63,tmp.getRow());
    ASSERT_EQ('P',tmp.getColumn());
    ASSERT_EQ("63P",tmp.getSeatId());

}

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

TEST(SeatStatus,ReadWriteBinary) {
    std::string id;
    SeatStatus tmp = loadRandomSeat(id);

    tmp.setStatus(DssApi::SeatStatus::TV_SVC_AVL, 1);
    tmp.setMode(DssApi::SeatStatus::Mode::KID,1);
    tmp.setUIState(DssApi::SeatStatus::MAP);

    ByteBuffer buffer;
    tmp.write(id,buffer);

    SeatStatus  tmp2;
    std::string id2;
    ByteBuffer::const_iterator it = buffer.cbegin();
    tmp2.read(it,id2);

    ASSERT_EQ(tmp,tmp2);
    ASSERT_EQ(id,id2);
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