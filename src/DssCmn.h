//
// Created by bstil on 7/1/2024.
//

#ifndef DSS_IF_DSSCMN_H
#define DSS_IF_DSSCMN_H

#include <stdint.h>
#include <stdexcept>
#include <vector>

namespace DssApi {
    typedef std::vector<uint8_t> ByteBuffer;

#define BIT_MASK(numbits)  ((1<<(numbits))-1)
#define BIT_MASK_AT(numbits, off)   (BIT_MASK(numbits) << (off))

    inline uint8_t getField(const uint8_t &field, int numbits, int offset) {
        uint8_t mask = BIT_MASK(numbits);
        uint8_t res = (field >> offset) & mask;
        return res;
    }

    inline uint8_t setField(uint8_t &field, int numbits, int offset, uint8_t value) {
        uint8_t mask = BIT_MASK_AT(numbits, offset);
        uint8_t tmp = value << offset;
        field = (field & ~mask) | (tmp & mask);

        return field;
    }

    class ParseException : std::runtime_error {
    public:
        ParseException(const std::string& msg = "");
    };

}
#endif //DSS_IF_DSSCMN_H
