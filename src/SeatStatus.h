//
// Created by bstil on 7/1/2024.
//

#ifndef DSS_IF_SEATSTATUS_H
#define DSS_IF_SEATSTATUS_H

#include <string>
#include <stdint.h>

#include "DssCmn.h"


namespace DssApi {

    class SeatStatus {
    public:
        typedef enum {
            DSS = 7,
            PR_AVL = 6,
            TV_SVC = 5,
            VLS = 4
        } Availability;

        typedef enum {
            KID = 7
        } Mode;

        typedef enum {
            MAP_VIEW
        } UIState;

        SeatStatus();
        SeatStatus(const std::string& seatId);

        void setSeatId(const std::string& id);
        std::string getSeatId() const;
        uint16_t getRow() const;
        char getColumn() const;

        uint8_t getStatus(Availability st) const;
        void    setStatus(Availability st,int val);

        uint8_t getMode(Mode m) const;
        uint8_t setMode(Mode m,uint8_t val);

        UIState  getUIState() const;
        void     setUIState(UIState s);

        ByteBuffer::const_iterator& loadBinary(ByteBuffer::const_iterator& it) ;

        void randomize();
        size_t  write(ByteBuffer& buf) const;

        bool operator==(const SeatStatus& rhs) const;
        bool operator!=(const SeatStatus& rhs) const {return !(*this == rhs);}
    private:
        uint8_t     seatRow;
        uint8_t     seatCol;
        uint8_t     status;
        uint8_t     mode;
    };

} // DssApi

#endif //DSS_IF_SEATSTATUS_H
