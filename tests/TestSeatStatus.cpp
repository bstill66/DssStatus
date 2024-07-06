//
// Created by bstil on 7/1/2024.
//

#include <gtest/gtest.h>
#include <random>
#include <format>

#include "SeatStatus.h"
#include "JsonUtils.h"

using namespace DssApi;

static std::default_random_engine generator;
static std::uniform_int_distribution<uint8_t> rowDistro(1,63);
static std::uniform_int_distribution<uint8_t> colDistro('A','P');
static std::uniform_int_distribution<uint8_t> binary(0,1);
static std::uniform_int_distribution<uint8_t> uiState(0,15);

static SeatStatus loadRandomSeat() {
    uint16_t row = rowDistro(generator);
    uint16_t col = colDistro(generator);
    auto seatId = std::format("{:d}{:c}",row,col);

    SeatStatus   tmp(seatId);
    tmp.setStatus(DssApi::SeatStatus::DSS,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::TV_SVC,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::PR_AVL,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::VLS,binary(generator));

    tmp.setMode(DssApi::SeatStatus::Mode::KID,binary(generator));
    tmp.setUIState(static_cast<DssApi::SeatStatus::UIState>(uiState(generator)));

    return tmp;
}
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
    tmp.setStatus(DssApi::SeatStatus::DSS,1);
    ASSERT_NE(tmp,tmp2);
    tmp.setStatus(DssApi::SeatStatus::DSS,0);
    ASSERT_EQ(tmp,tmp2);

    ASSERT_NE(tmp,tmp3);
}

TEST(SeatStatus,ReadWriteBinary) {
    SeatStatus tmp("23A");
    tmp.setStatus(DssApi::SeatStatus::PR_AVL,1);
    tmp.setStatus(DssApi::SeatStatus::TV_SVC,1);
    tmp.setMode(DssApi::SeatStatus::Mode::KID,1);
    tmp.setUIState(DssApi::SeatStatus::MAP_VIEW);

    ByteBuffer buffer;
    tmp.write(buffer);

    SeatStatus  tmp2;
    ByteBuffer::const_iterator it = buffer.cbegin();
    tmp2.loadBinary(it);

    ASSERT_EQ(tmp,tmp2);
}

TEST(SeatStatus,ReadWriteJSON) {
    using namespace DssApi::JsonUtils;
    using namespace std;

    for (int n=0;n<100;n++) {
        SeatStatus tmp = loadRandomSeat();
        SeatStatus tmp2;

        ASSERT_NE(tmp,tmp2);

        JSon   obj;
        toJson(tmp,obj);

        auto ans = to_string(obj);

        fromJson(obj,tmp2);
        ASSERT_EQ(tmp,tmp2);

        auto str = toJson(tmp2);
        SeatStatus tmp3;
        ASSERT_NE(tmp,tmp3);
        fromJson(str,tmp3);
        ASSERT_EQ(tmp,tmp3);

    }
}