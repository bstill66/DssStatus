//
// Created by bstil on 7/1/2024.
//

#include "DssStatus.h"
#include "JsonUtils.h"

#include "base64pp/base64pp.h"

namespace DssApi {

using namespace nlohmann;
using namespace JsonUtils;

DssStatus::DssStatus()
: seats()
{}

DssStatus::DssStatus(const std::string &str) {

    JSon obj = json::parse(str);
    JsonUtils::buildFromJson(obj,*this);
}

DssStatus::DssStatus(std::istream &infile) {
    JSon obj = json::parse(infile);

    JsonUtils::buildFromJson(obj,*this);
}

#if 0
void DssStatus::randomize(int numSeats) {
    dssStatus.randomize();
    for (int i = 0; i < numSeats; i++) {
        SeatStatus stat;
        stat.randomize();
        seats.push_back(stat);
    }
}
#endif




std::string
DssStatus::asBase64() const {


    std::vector<uint8_t> buffer;


    size_t size = 0;
    size += dssStatus.append(buffer);

    for (auto item: seats) {
        size += item.write(buffer);
    }


    auto const base64_str = base64pp::encode({begin(buffer), end(buffer)});
    size_t binSize = buffer.size();
    size_t actSize = base64_str.length();

    return base64_str;
}

bool DssStatus::operator==(const DssStatus &rhs) const {
    size_t tmp1 = seats.size(), tmp2 = rhs.seats.size();

    return (dssStatus == rhs.dssStatus) &&
           (seats == rhs.seats);
}

bool DssStatus::fromBase64(const std::string &b64) {

    auto buffer = base64pp::decode(b64);
    if (buffer != std::nullopt) {
        size_t binSize = buffer->size();
        std::vector<uint8_t>::const_iterator iter = buffer->begin();

        if (dssStatus.loadBinary(iter) != buffer->end()) {
            seats.clear();
            while (iter != buffer->end()) {
                SeatStatus tmp;
                tmp.loadBinary(iter);
                seats.push_back(tmp);
            }

            return true;
        }

    }

    return false;
}

}