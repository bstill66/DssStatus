//
// Created by bstill on 8/2/24.
//

#include <gtest/gtest.h>


#include "DssCmn.h"
#include "SeatID.h"

using namespace DssApi;
using namespace std;

TEST(SeatID,Constructors) {

    SeatID   tmp;
    ASSERT_FALSE(tmp.isValid());

    SeatID   _1a("1A");
    SeatID   other(1,'A');
    ASSERT_EQ(_1a,other);
    ASSERT_FALSE(_1a != other);

    ASSERT_THROW({SeatID  tmp("64A");},InvalidSeatID);
    ASSERT_THROW({SeatID  tmp2("0A");},InvalidSeatID);
    ASSERT_THROW({SeatID  tmp3("1Q");},InvalidSeatID);
    ASSERT_THROW({SeatID  tmp4("");},InvalidSeatID);
    ASSERT_THROW({SeatID  tmp5("1");},InvalidSeatID);
    ASSERT_THROW({SeatID  tmp6("1%");},InvalidSeatID);
    ASSERT_THROW({SeatID  tmp7("1a");},InvalidSeatID);
    ASSERT_NO_THROW({SeatID tmp4("1O");});

}

TEST(SeatID,GetSet) {

    SeatID  tmp("1A");
    ASSERT_EQ(tmp.getRow(),1);
    ASSERT_EQ(tmp.getColumn(),'A');
    ASSERT_TRUE(tmp.isValid());

    SeatID   tmp2;
    ASSERT_FALSE(tmp2.isValid());

}