//
// Created by bstil on 7/1/2024.
//

#include <random>
#include <iostream>

#include "nlohmann/json.hpp"
#include "ServerStatus.h"
#include "JsonUtils.h"

namespace DssApi {

ServerStatus::ServerStatus()
{
    clear();
}

void ServerStatus::clear() {
    size         = 0x84;
    availability = 0;
    upTime       = 0;
    reserved     = 0;
}

void ServerStatus::setAvailability(ServerStatus::Name id, bool state) {
    setField(availability,1,id,state?1:0);
}

uint8_t ServerStatus::getAvailability(ServerStatus::Name id) const {
    return getField(availability,1,id);
}

size_t ServerStatus::append(ByteBuffer &buf) const {

    size_t start = buf.size();
    buf.push_back(size);
    buf.push_back(availability);
    buf.push_back(reserved);
    buf.push_back(upTime);

    return buf.size()-start;
}

#if 0
void ServerStatus::randomize() {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(0,255);

    availability = distribution(generator);
    upTime       = distribution(generator);
    pairingCount = distribution(generator);
    reserved     = 0;

}
#endif

bool ServerStatus::operator==(const ServerStatus &rhs) const {
    return ((availability   == rhs.availability) &&
            (upTime         == rhs.upTime)       &&
            (size           == rhs.size) &&
            (reserved       == rhs.reserved));
}


ByteBuffer::const_iterator&
ServerStatus::loadBinary(ByteBuffer::const_iterator &it) {
    size         = *it++;
    availability = *it++;
    reserved     = *it++;
    upTime       = *it++;

    return it;
}


void to_json(JSon& j, const ServerStatus& s) {
    using namespace DssApi;

    j = JSon{{"INT_AVL", s.getAvailability(ServerStatus::INT_AVL)},
             {"PR_AVL" , s.getAvailability(ServerStatus::PR_AVL)},
             {"TV_AVL",  s.getAvailability(ServerStatus::TV_AVL)},
             {"LPNS_AVL",s.getAvailability(ServerStatus::LPNS_AVL)},
             {"FLT_OPN", s.getAvailability(ServerStatus::FLT_OPN)},
             {"UpTime",  s.getUpTime()}};

}

void from_json(const JSon& j, ServerStatus& s) {
    using namespace JsonUtils;

        s.clear();

        s.setAvailability(ServerStatus::INT_AVL,getBoolean(j,"INT_AVL"));
        s.setAvailability(ServerStatus::PR_AVL,getBoolean(j,"PR_AVL"));
        s.setAvailability(ServerStatus::TV_AVL,getBoolean(j,"TV_AVL"));
        s.setAvailability(ServerStatus::LPNS_AVL,getBoolean(j,"LPNS_AVL"));
        s.setAvailability(ServerStatus::FLT_OPN,getBoolean(j,"FLT_OPN"));
        s.setUpTime(j.at("UpTime"));
}
} // DssApi