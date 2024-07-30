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
            DSS_COMM_LOSS = 7,
            TM_SYNC       = 6,
            TV_SVC_AVL = 5,
            VLS = 4,
            PA = 3,
            PCTL_LOCK = 2
        } Availability;


        typedef enum {
            LOGIN_AVL   = 7,
            LOGD_IN     = 6,
            KID         = 5
        } Mode;

        typedef enum {
            WELCOME   = 0,
            HOME      = 1,
            PAIRING   = 2,
            WATCH_TV  = 3,
            STREAM_VIDEO = 4,
            STREAM_AUDIO = 5,
            PLAY_GAME    = 6,
            MAP          = 7,
            SETTINGS     = 8,
            UNSPECIFIED  = 15
        } UIState;

        SeatStatus();
        SeatStatus(const std::string& seatId);

        void clear();


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

        ByteBuffer::const_iterator& read(ByteBuffer::const_iterator& it, std::string& id) ;

        void randomize();
        size_t  write(const std::string& id,ByteBuffer& buf) const;

        bool operator==(const SeatStatus& rhs) const;
        bool operator!=(const SeatStatus& rhs) const {return !(*this == rhs);}
    protected:
        static void convertId(const std::string& id,uint8_t& row,uint8_t& col);
        static void convertId(uint8_t row, uint8_t col, std::string& id);
    private:
        uint8_t     seatRow;
        uint8_t     seatCol;
        uint8_t     status;
        uint8_t     mode;
    };

    void to_json(JSon& j, const SeatStatus& s) ;
    void from_json(const JSon& j, SeatStatus& s);
} // DssApi

#endif //DSS_IF_SEATSTATUS_H
