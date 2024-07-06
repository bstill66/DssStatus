//
// Created by bstil on 7/1/2024.
//

#include <random>

#include "ServerStatus.h"

namespace DssApi {

ServerStatus::ServerStatus()
{
    clear();
}

void ServerStatus::clear() {
    availability = 0;
    upTime       = 0;
    pairingCount = 0;
    reserved     = 0;
}

void ServerStatus::setAvailability(ServerStatus::Name id, bool state) {
    setField(availability,1,id,state?1:0);
}

bool ServerStatus::getAvailability(ServerStatus::Name id) const {
    return getField(availability,1,id);
}

size_t ServerStatus::append(ByteBuffer &buf) const {

    size_t start = buf.size();

    buf.push_back(availability);
    buf.push_back(upTime);
    buf.push_back(pairingCount);
    buf.push_back(reserved);

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
            (pairingCount   == rhs.pairingCount) &&
            (reserved       == rhs.reserved));
}


ByteBuffer::const_iterator&
ServerStatus::loadBinary(ByteBuffer::const_iterator &it) {
    availability = *it++;
    upTime       = *it++;
    pairingCount = *it++;
    reserved     = *it++;

    return it;
}

} // DssApi