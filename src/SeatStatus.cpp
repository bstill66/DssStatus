//
// Created by bstil on 7/1/2024.
//

#include <regex>
#include <random>
#include <format>

#include "base64pp/base64pp.h"
#include "SeatStatus.h"
#include "JsonUtils.h"

static const std::regex SEAT_ID_RE("(\\d{1,2})([A-P]{1})");

namespace DssApi {

SeatStatus::SeatStatus()
        : seatRow(0), seatCol(0), status(0), mode(0)
{}

SeatStatus::SeatStatus(const std::string &seatId)
        : seatRow(0), seatCol(0), status(0), mode(0)
{
    setSeatId(seatId);
}

void SeatStatus::clear() {
    seatRow = 0;
    seatCol = 0;
    status  = 0;
    mode    = 0;
}

void SeatStatus::convertId(const std::string& id,uint8_t& row,uint8_t& col) {
    using namespace std;

    bool valid = false;

    smatch   matches;
    if (regex_match(id,matches,SEAT_ID_RE) ) {
        std::string tmpRow = matches[1];
        std::string tmpCol = matches[2];

        uint8_t   rowInt = atoi(tmpRow.c_str());
        uint8_t   colInt = tmpCol.at(0) - 'A';

        if ((rowInt > 0) && (rowInt < 64) && (colInt <= 15)) {
            row = rowInt;
            col = colInt;
            valid = true;
        }
    }

    if (!valid)  {
        throw std::runtime_error("Invalid Seat ID: " + id);
    }
}
void SeatStatus::setSeatId(const std::string& id) {
    using namespace std;

    bool valid = false;

    smatch   matches;
    if (regex_match(id,matches,SEAT_ID_RE) ) {
        std::string row = matches[1];
        std::string col = matches[2];

        uint8_t   rowInt = atoi(row.c_str());
        uint8_t   colInt = col.at(0) - 'A';

        if ((rowInt > 0) && (rowInt < 64) && (colInt <= 15)) {
            setField(seatRow,6,0,rowInt);
            setField(seatCol,4,3,colInt);
            valid = true;
        }
    }

    if (!valid)  {
        throw std::runtime_error("Invalid Seat ID: " + id);
    }
}

uint16_t SeatStatus::getRow() const {
   return getField(seatRow,6,0);
}

char SeatStatus::getColumn() const {
   uint8_t  col = getField(seatCol,4,4);
   return 'A'+ col;
}

std::string
SeatStatus::getSeatId() const {
    return std::format("{}{:1}",getRow(),getColumn());
}
void SeatStatus::convertId(uint8_t row, uint8_t col, std::string &id) {
    id = std::format("{:1d}{:1c}",row,static_cast<char>('A'+col));

}

uint8_t SeatStatus::getStatus(SeatStatus::Availability st) const {
    return getField(status,1,st);
}

void SeatStatus::setStatus(SeatStatus::Availability st, int val) {
    status = setField(status,1,st,val);
}

uint8_t SeatStatus::getMode(Mode m) const {
    return getField(mode,1,m);
}

uint8_t SeatStatus::setMode(Mode m,uint8_t val) {
    return setField(mode,1,m,val);
}

void SeatStatus::setUIState(DssApi::SeatStatus::UIState s) {
    setField(mode,4,0,s);
}

SeatStatus::UIState
SeatStatus::getUIState() const {
    return static_cast<UIState>(getField(mode,4,0));
}

size_t SeatStatus::write(const std::string& id,ByteBuffer &buf) const {
    size_t start = buf.size();

    uint8_t  row = 0,col = 0;
    convertId(id,row,col);
    buf.push_back(row);
    buf.push_back(col<<4);

    buf.push_back(status);
    buf.push_back(mode);

    return buf.size()-start;
}

#if 0
void SeatStatus::randomize() {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(0,255);

    seatRow = distribution(generator);
    seatCol = distribution(generator);
    status  = distribution(generator);
    mode    = distribution(generator);
}
#endif

std::vector<uint8_t>::const_iterator&
SeatStatus::read(ByteBuffer::const_iterator& it,std::string& id) {

    uint8_t  row,col;
    row     = *it++;
    col     = *it++;
    convertId(row,col>>4,id);

    seatRow = row;
    seatCol = col;
    status  = *it++;
    mode    = *it++;

    return it;
}

bool SeatStatus::operator==(const SeatStatus &rhs) const {
    return (seatRow      == rhs.seatRow) &&
           (seatCol     == rhs.seatCol) &&
           (status      == rhs.status) &&
           (mode        == rhs.mode);
}


    void to_json(JSon& j, const SeatStatus& s) {
        using namespace DssApi;

        j = {//{"SeatId",       s.getSeatId()},
             {"DSS_COMM_LOSS",s.getStatus(SeatStatus::DSS_COMM_LOSS)},
             {"TM_SYNC",      s.getStatus(SeatStatus::TM_SYNC)},
             {"TV_SVC_AVL",   s.getStatus(SeatStatus::TV_SVC_AVL)},
             {"VLS",          s.getStatus(SeatStatus::VLS)},
             {"PA",           s.getStatus(SeatStatus::PA)},
             {"PCTL_LOCK",    s.getStatus(SeatStatus::PCTL_LOCK)},

             {"KID",          s.getMode(SeatStatus::KID)},
             {"LOGIN_AVL",    s.getMode(SeatStatus::LOGIN_AVL)},
             {"LOGD_IN",      s.getMode(SeatStatus::LOGD_IN)},
             {"UI",           s.getUIState()}};

    }

    void from_json(const JSon& j, SeatStatus& s) {
        using namespace JsonUtils;

        s.clear();

        //s.setSeatId(j["SeatId"]);
        s.setStatus(SeatStatus::DSS_COMM_LOSS,getBoolean(j,"DSS_COMM_LOSS"));
        s.setStatus(SeatStatus::TM_SYNC,getBoolean(j,"TM_SYNC"));
        s.setStatus(SeatStatus::TV_SVC_AVL,getBoolean(j,"TV_SVC_AVL"));
        s.setStatus(SeatStatus::VLS,getBoolean(j,"VLS"));
        s.setStatus(SeatStatus::PA,getBoolean(j,"PA"));
        s.setStatus(SeatStatus::PCTL_LOCK,getBoolean(j,"PCTL_LOCK"));

        s.setMode(SeatStatus::LOGIN_AVL,getBoolean(j,"LOGIN_AVL"));
        s.setMode(SeatStatus::LOGD_IN,getBoolean(j,"LOGD_IN"));
        s.setMode(SeatStatus::KID,getBoolean(j,"KID"));
        s.setUIState(static_cast<SeatStatus::UIState>(j["UI"]));

    }
} // DssApi