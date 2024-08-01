//
// Created by bstill on 7/30/24.
//

#include <format>
#include <regex>
#include "SeatID.h"

namespace DssApi {
    static const std::regex SEAT_ID_RE("(\\d{1,2})([A-P]{1})");

    const uint8_t SeatID::INVALID_VAL = -1;
    const uint8_t SeatID::MIN_ROW     = 1;
    const uint8_t SeatID::MAX_ROW     = 63;
    const char    SeatID::MAX_COL     = 'A' + 15;

    SeatID::SeatID()
    : row(INVALID_VAL),col(INVALID_VAL)
    {}

    SeatID::SeatID(const std::string &id)
        : row(INVALID_VAL),col(INVALID_VAL)
    {
        fromString(id,row,col);
    }

    SeatID::SeatID(uint8_t row, char col)
    : row(INVALID_VAL),col(INVALID_VAL)
    {

    }

    SeatID::operator std::string() const {
        return std::format("{:1d}{:1c}",row,col);
    }

    void SeatID::get(uint8_t row, uint8_t col) {


        }

    void SeatID::fromString(const std::string& id,uint8_t& r,char& c) {
        using namespace std;

        bool valid = false;

        smatch   matches;
        if (regex_match(id,matches,SEAT_ID_RE) ) {
            std::string tmpRow = matches[1];

            uint8_t   rowInt = atoi(tmpRow.c_str());
            if ((rowInt >= MIN_ROW) && (rowInt <= MAX_ROW)) {
                std::string tmpCol = matches[2];
                uint8_t   colInt = tmpCol.at(0) - 'A';
                if ((tmpCol.length() == 1) && (colInt <= MAX_COL)) {
                    r = rowInt;
                    c = tmpCol.at(0);
                    valid = true;
                }
            }
        }

        if (!valid)  {
            throw std::runtime_error(format("Invalid Seat ID: '{}'" ,id));
        }
    }


    bool SeatID::isValid() const {
        return ((row <= MAX_ROW) && (col >= 'A') && (col <= MAX_COL));
    }

}
