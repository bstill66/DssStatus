//
// Created by bstill on 7/30/24.
//

#ifndef SEATID_H
#define SEATID_H

#include <cstdint>
#include <string>

namespace DssApi {

 class SeatID {
 public:
     SeatID();
     SeatID(const std::string& id);
     SeatID(uint8_t row, char col);

     void get(uint8_t row, uint8_t col);
     operator std::string() const;

     uint8_t getRow() const {return row;}
     char    getColumn() const {return col;}

     bool isValid() const;
 protected:
     bool isValid(uint8_t r,char c) const;
     void fromString(const std::string& id,uint8_t& r,char& c);
 private:
     static const uint8_t  INVALID_VAL;
     static const uint8_t  MIN_ROW;
     static const uint8_t  MAX_ROW;
     static const char     MAX_COL;

     uint8_t    row;
     char       col;

 };
}
#endif //SEATID_H
