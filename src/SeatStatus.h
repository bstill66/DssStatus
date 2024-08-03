//
// Created by bstil on 7/1/2024.
//

#ifndef DSS_IF_SEATSTATUS_H
#define DSS_IF_SEATSTATUS_H

#include <string>
#include <stdint.h>

#include "DssCmn.h"


namespace DssApi {
    /**
     * This class contains data structures related to the status of a specific seat.
     */
    class SeatStatus {
    public:
        /**
         * This typedef defines the various Availablity status values associated with a seat.
         *
         * Note: These values are used as bit offsets in storing binary data elements. They should each be
         * unique and/or less than 8.
         */
        typedef enum {
            DSS_COMM_LOSS = 7,
            TM_SYNC       = 6,
            TV_SVC_AVL = 5,
            VLS = 4,
            PA = 3,
            PCTL_LOCK = 2,
            STOWD = 1
        } Availability;


        /**
         * This typedef defines mode values that are used in SeatStatus.
         * Note: They are used for bit offsets within the binary represenation.
         */
        typedef enum {
            LOGIN_AVL   = 7,
            LOGD_IN     = 6,
            KID         = 5
        } Mode;

        /**
         * These values are used for specifying the UI information related to the seat.
         */
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

        /**
         * Default constructor
         */
        SeatStatus();

        /**
         * Clear all values of the SeatStatus to their default (i.e., 0) values.
         */
        void clear();

        /**
         * Get the Availability status value specified.
         *
         * @param st Status indicator to retrieve
         * @return value associated with the availability
         */
        uint8_t getStatus(Availability st) const;

        /**
         *  Set the Availability status element to the specified value.
         */
        void    setStatus(Availability st,int val);

        /**
         * Retrieve the specified mode value
         *
         * @param m mode to retrieve
         * @return value of the specified mode
         */
        uint8_t getMode(Mode m) const;
        uint8_t setMode(Mode m,uint8_t val);

        /**
         * Retrieve the UI state information.
         *
         * @return Current UI state
         */
        UIState  getUIState() const;
        void     setUIState(UIState s);

        /**
         * Read the buffer into "this" object
         *
         * @param it buffer iterator
         * @return number of bytes read
         */
        ByteBuffer::const_iterator& read(ByteBuffer::const_iterator& it) ;

        /**
         * Write "this" object into the buffer
         *
         * @param buf buffer to write into
         * @return number of bytes written
         */
        size_t  write(ByteBuffer& buf) const;

        /**
         * Equality operator
         *
         * @param rhs object to compare to
         * @return true if equal
         */
        bool operator==(const SeatStatus& rhs) const;
        bool operator!=(const SeatStatus& rhs) const {return !(*this == rhs);}
    protected:
    private:
        uint8_t     status;    // byte field containing status
        uint8_t     mode;      // byte field containnig mode info
    };

    // Helper functions to read/write as JSON
    void to_json(JSon& j, const SeatStatus& s) ;
    void from_json(const JSon& j, SeatStatus& s);
} // DssApi

#endif //DSS_IF_SEATSTATUS_H
