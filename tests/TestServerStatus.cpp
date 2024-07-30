//
// Created by bstil on 7/2/2024.
//
#include <gtest/gtest.h>
#include <random>
#include <format>

#include "ServerStatus.h"
#include "JsonUtils.h"

using namespace DssApi;

ServerStatus loadRandomStatus();


TEST(ServerStatus, Constructors) {


}

TEST(ServerStatus,operators) {

    ServerStatus tmp = loadRandomStatus();
    ServerStatus tmp2= loadRandomStatus();

    ASSERT_NE(tmp,tmp2);

    ServerStatus tmp3 = tmp;
    ASSERT_EQ(tmp3,tmp);
}

TEST(ServerStatus,BinaryFormat) {

    for (int i=0;i<100;i++) {
        ServerStatus  tmp = loadRandomStatus();

        ByteBuffer buf;

        size_t sz = tmp.append(buf);

        ServerStatus  tmp2;
        ByteBuffer::const_iterator it = buf.cbegin();
        tmp2.loadBinary(it);

        ASSERT_EQ(it-buf.cbegin(),buf.size());
        ASSERT_EQ(tmp,tmp2);
    }
}


TEST(ServerStatus,SetGet) {
    using namespace DssApi;

    ServerStatus::Name Avail[] = {ServerStatus::INT_AVL,ServerStatus::PR_AVL,ServerStatus::TV_AVL};
    ServerStatus tmp;

    for (int i=0;i<sizeof(Avail)/sizeof(Avail[0]);i++) {
        ASSERT_NE(tmp.getAvailability(Avail[i]), true);
        tmp.setAvailability(Avail[i],true);
        ASSERT_EQ(tmp.getAvailability(Avail[i]),true);
    }
}

TEST(ServerStatus,ReadriteJSON)
{

    using namespace DssApi::JsonUtils;
    using namespace std;

    for (int n=0;n<100;n++) {
        ServerStatus tmp = loadRandomStatus();
        ServerStatus tmp2;

        ASSERT_NE(tmp, tmp2);

        JSon obj;
        to_json(obj,tmp);
        auto strj = to_string(obj);

        from_json(obj,tmp2);
        ASSERT_EQ(tmp, tmp2);

#if 0
        auto str = tmp2);
        ServerStatus tmp3;
        ASSERT_NE(tmp, tmp3);
        fromJson(str, tmp3);
        ASSERT_EQ(tmp, tmp3);
#endif
    }
}