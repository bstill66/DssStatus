//
// Created by bstill on 7/30/24.
//

#include <format>
#include <regex>
#include "SeatID.h"

namespace DssApi {


    const uint8_t SeatID::INVALID_VAL = -1;
    const uint8_t SeatID::MIN_ROW     = 1;
    const uint8_t SeatID::MAX_ROW     = 63;
    const char    SeatID::MIN_COL     = 'A';
    const char    SeatID::MAX_COL     = SeatID::MIN_COL + 15;

    // Regular expression to validate row and column string representations
    static const std::regex SEAT_ID_RE(std::format("(\\d{{1,2}})([{:1c}-{:1c}]{{1}})",SeatID::MIN_COL,SeatID::MAX_COL));

    /**
     * Default constructor
     */
    SeatID::SeatID()
    : row(INVALID_VAL),col(INVALID_VAL)
    {}

    /**
     * Construct from string
     * @param id string version of identifier
     */
    SeatID::SeatID(const std::string &id)
        : row(INVALID_VAL),col(INVALID_VAL)
    {
        // Simply call internal converter
        fromString(id,row,col);
    }

    /**
     * Construct from a string
     * @param str value to convert from
     */
    SeatID::SeatID(const char *str)
        : row(INVALID_VAL),col(INVALID_VAL) {
        // Simply call internal converter
        fromString(str,row,col);
    }

    /**
     * Construct from specific row & column
     *
     * @param r row to set
     * @param c column to set
     */
    SeatID::SeatID(uint8_t r, char c)
    : row(INVALID_VAL),col(INVALID_VAL)
    {
        // Only update if valid
        if (isValid(r,c)) {
            row = r;
            col = c;
        }
    }

    /**
     * Convert to a string version
     */
    SeatID::operator std::string() const {
        return std::format("{:1d}{:1c}",row,col);
    }


    /**
     * Check to see if values are valid
     *
     * @param row row to validate
     * @param col column to validate
     * @return true if valie
     */
    bool SeatID::isValid(uint8_t row,char col) {
        // Return true if the row and colunmns are both within the
        // specified ranges allowed.
        return ((row >= MIN_ROW) && (row <= MAX_ROW) &&
                (col >= MIN_COL)    && (col <= MAX_COL));
    }

    /**
     * Convert from string to internal representation.
     * Invalid range will throw an InvalidSeatID exception.
     * @param id identifier in <row><col> format
     * @param r row that was extracted
     * @param c column that was extracted
     */
    void SeatID::fromString(const std::string& id,uint8_t& r,char& c) {
        using namespace std;

        // Assume not valid
        bool valid = false;

        //*******
        // Use regular expressions to extract components
        //******
        smatch   matches;
        if (regex_match(id,matches,SEAT_ID_RE) ) {
            // Convert the row component into an integer
            std::string tmpRow = matches[1];
            uint8_t   rowInt = atoi(tmpRow.c_str());

            // Make sure it is within valid range
            if ((rowInt >= MIN_ROW) && (rowInt <= MAX_ROW)) {
                // Get the column component and convert to a character
                std::string tmpCol = matches[2];
                uint8_t   colInt = tmpCol.at(0);

                if ((tmpCol.length() == 1) && (colInt >= MIN_COL) && (colInt <= MAX_COL)) {
                    // Everything looks good...load up the extracted values
                    r = rowInt;
                    c = tmpCol.at(0);

                    // Mark as valid
                    valid = true;
                }
            }
        }

        // If not valid...throw exception
        if (!valid)  {
            throw InvalidSeatID(format("{}" ,id));
        }
    }

    /**
     * Check to see if this object is valid
     *
     * @return true if valie, false otherwise
     */
    bool SeatID::isValid() const {
        return isValid(row,col);
    }

    /**
     *  Write the contents of this object into the buffer
     * @param buf buffer to write to
     * @return number of bytes written
     */
    size_t SeatID::write(ByteBuffer &buf) const {
        buf.push_back(row);
        buf.push_back((col-'A')<<4);

        return 2;
    }

    /**
     * Read the contents of the buffer into "this" object
     * @param buf buffer to read from
     * @return number of bytes read
     */
    size_t SeatID::read(ByteBuffer::const_iterator& buf) {
        row = *buf++;
        col = 'A' + (*buf++ >> 4);

        return 2;
    }

    /**
     * Less than operator
     *
     * @param rhs object to compare against
     * @return true if less than
     */
    bool SeatID::operator<(const SeatID &rhs) const {
        // If rows are equal..then look at column
        if (row == rhs.row)
             return col < rhs.col;

        // otherwise return the lesser row
        return row < rhs.row;
    }

    /**
     * Equality operator
     *
     * @param rhs object to compare against
     * @return true if equal, false otherwise
     */
    bool SeatID::operator==(const SeatID &rhs) const {
        return (row == rhs.row) && (col == rhs.col);
    }

}
