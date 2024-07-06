//
// Created by bstil on 7/1/2024.
//

#include <regex>
#include <random>
#include <format>

#include "base64pp/base64pp.h"
#include "SeatStatus.h"

static const std::regex SEAT_ID_RE("(\\d{1,2})([A-P]{1})");

namespace DssApi {

SeatStatus::SeatStatus()
        : seatRow(0), seatCol(0), status(0), mode(0)
{

}

SeatStatus::SeatStatus(const std::string &seatId)
        : seatRow(0), seatCol(0), status(0), mode(0)
{
    setSeatId(seatId);
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
   uint8_t  col = getField(seatCol,4,3);
   return 'A'+ col;
}

std::string
SeatStatus::getSeatId() const {
    return std::format("{}{:1}",getRow(),getColumn());
}

uint8_t SeatStatus::getStatus(SeatStatus::Availability st) const {
    return getField(status,1,st);
}

void SeatStatus::setStatus(SeatStatus::Availability st, int val) {
    status = setField(status,1,st,val);
}

uint8_t SeatStatus::getMode(Mode m) const {
    return getField(mode,1,7);
}

uint8_t SeatStatus::setMode(Mode m,uint8_t val) {
    return setField(mode,1,7,val);
}

void SeatStatus::setUIState(DssApi::SeatStatus::UIState s) {
    setField(mode,4,0,s);
}

SeatStatus::UIState
SeatStatus::getUIState() const {
    return static_cast<UIState>(getField(mode,4,0));
}

size_t SeatStatus::write(ByteBuffer &buf) const {
    size_t start = buf.size();

    buf.push_back(seatRow);
    buf.push_back(seatCol);
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
SeatStatus::loadBinary(std::vector<uint8_t>::const_iterator& it) {

    seatRow = *it++;
    seatCol = *it++;
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



} // DssApi