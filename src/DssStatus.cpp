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
: server(),seats()
{}

DssStatus::DssStatus(const std::string &str) {

    JSon obj = json::parse(str);
    from_json(obj,*this);

}

DssStatus::DssStatus(std::istream &infile) {
    JSon obj = json::parse(infile);

    from_json(obj,*this);

}

void DssStatus::clear() {
    server.clear();
    seats.clear();
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


    ByteBuffer buffer;


    size_t size = 0;
    size += server.append(buffer);

    for (auto item: seats) {
        // write the SeatID
        size += item.first.write(buffer);

        // write the seat status
        size += item.second.write(buffer);
    }

    auto const base64_str = base64pp::encode({begin(buffer), end(buffer)});
    size_t binSize = buffer.size();
    size_t actSize = base64_str.length();

    return base64_str;
}

bool DssStatus::operator==(const DssStatus &rhs) const {
    return (server == rhs.server) &&
           (seats == rhs.seats);
}

bool DssStatus::fromBase64(const std::string &b64) {

    auto buffer = base64pp::decode(b64);
    if (buffer != std::nullopt) {
        ByteBuffer::const_iterator iter = buffer->begin();

        if (server.loadBinary(iter) != buffer->end()) {
            seats.clear();
            while (iter != buffer->end()) {
                SeatID  id;
                id.read(iter);

                SeatStatus tmp;

                tmp.read(iter);
                seats[id] = tmp;
            }

            return true;
        }

    }

    return false;
}

void DssStatus::add(const SeatID& id,const SeatStatus& seat) {
    seats[id] = seat;
    //seats[id].setSeatId(id);
}

void DssStatus::set(const DssApi::ServerStatus &s) {
    server = s;
}

const DssStatus::SeatCollection& DssStatus::getSeatStatus() const {
    return seats;
}

const ServerStatus& DssStatus::getServerStatus() const {
    return server;
}



    void to_json(JSon& j, const DssStatus& s) {
        using namespace DssApi;

        JSon   jsrvr;
        to_json(jsrvr,s.getServerStatus());

        JSon    jSeats;
        to_json(jSeats,s.getSeatStatus());

        j = {{"Server", jsrvr},
             {"Seats",  jSeats}};

    }

    void from_json(const JSon& j, DssStatus& s) {
        using namespace JsonUtils;

        s.clear();

        ServerStatus  tmp;
        from_json(j["Server"],tmp);
        s.set(tmp);

        DssStatus::SeatCollection seats;
        from_json(j["Seats"],seats);

        for (auto itm : seats) {
            //itm.second.setSeatId(itm.first);
            s.add(itm.first,itm.second);
        }


    }
}