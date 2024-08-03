//
// Created by bstill on 7/30/24.
//

#ifndef SEATID_H
#define SEATID_H

#include <cstdint>
#include <string>

#include "DssCmn.h"

namespace DssApi {

    /***
     * This class encapsulates the logic and formatting associated with a SeatIdentifier.
     * A SeatID represents the row and column number of a seat.
     * It's valid range is set between MIN_ROW & MAX_ROW and MIN_COL & MAX_COL.
     ****/
 class SeatID {
 public:
     /**
      * Default Constructor
      * NOTE: SeatID will be considered "invalid"
      */
     SeatID();

     /**
      * This constructor pre-fills the seatID from a representative string. It will throw a
      * InvalidSeatID exception if it is invalid.
      *
      * @param id String representing the row/column (i.e., "1A" or "43F")
      */
     SeatID(const std::string& id);
     SeatID(const char *id);

     /**
      * This constructor will populate the SeatID from the specified row and column info.
      * An InvalidSeatID exception will be thrown if invalid
      * @param row  row number of the seat (1-MAX_ROW)
      * @param col  column "character"   'A' - MAX_COL
      */
     SeatID(uint8_t row, char col) ;

     /**
      * Convert the SeatID to a human readable string
      */
     operator std::string() const;

     /**
      * Get the ordinal row value
      * @return row value
      */
     uint8_t getRow() const {return row;}

     /**
      * Return the Column value
      * @return column value
      */
     char    getColumn() const {return col;}

     /**
      * Query the seatID to determine if it is valid (or not)
      * @return true if valid, false otherwise
      */
     bool isValid() const;

     /**
      * Write the contents of the SeatID to the specified buffer
      * @param buf buffer to write to
      * @return number of bytes written
      */
     size_t  write(ByteBuffer& buf) const;

     /**
      * Read the contents from the buffer and return number of bytes consumed.
      * @param buf Buffer to be read from
      * @return number of bytes read from buffer
      */
     size_t  read(ByteBuffer::const_iterator& buf);

     /**
      * Equality operator
      * @param rhs right-hand side object to compare "this" to
      * @return true if equal, false otherwise
      */
     bool operator==(const SeatID& rhs) const;
     bool operator!=(const SeatID& rhs) const {return !(*this == rhs);}

     /**
      * Less than operator - used for ordering within map/sets
      * @param rhs right-hand object to compare to
      * @return true if less than rhs
      */
     bool operator<(const SeatID& rhs) const;

     static const uint8_t  MIN_ROW;
     static const uint8_t  MAX_ROW;
     static const char     MIN_COL;
     static const char     MAX_COL;

 protected:
     /**
      * Check to determine if specified parameters are valid or not
      * @param r row to use in validation check
      * @param c column to use in validation check
      * @return true if valid, false otherwise
      */
     static bool isValid(uint8_t r,char c);

     /**
      * Convert string value into corresponding internal representation.
      * @param id identifier to convert
      * @param r parameter to store row value into (if valid)
      * @param c parameter to store column value into (if valid)
      */
     void fromString(const std::string& id,uint8_t& r,char& c);
 private:
     static const uint8_t  INVALID_VAL;

     uint8_t    row;      // row number (1..MAX_ROW)
     char       col;      // column "character" (A..MAX_COL)

 };
}
#endif //SEATID_H
