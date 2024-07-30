//
// Created by bstil on 7/1/2024.
//

#ifndef DSS_IF_SERVERSTATUS_H
#define DSS_IF_SERVERSTATUS_H

#include "DssCmn.h"

namespace DssApi {

    class ServerStatus {

        public:
        ServerStatus();

            typedef enum {
                FLT_OPN  = 7,
                INT_AVL  = 6,
                PR_AVL   = 5,
                TV_AVL   = 4,
                LPNS_AVL = 3
            } Name;

            void clear();

            void setAvailability(Name id, bool state);
            uint8_t getAvailability(Name id) const;

            void setUpTime(uint16_t ut) { upTime = ut;}
            uint16_t getUpTime() const {return upTime;}


            uint16_t getLoggedInCount() const;

            size_t  append(ByteBuffer& buf) const;

            ByteBuffer::const_iterator& loadBinary(ByteBuffer::const_iterator & it);
            void randomize();

            bool operator==(const ServerStatus& rhs) const;
            bool operator!=(const ServerStatus& rhs) const {return !(*this == rhs);}
        protected:
            uint8_t       size;
            uint8_t       availability;
            uint8_t       reserved;
            uint8_t       upTime;
    };

    void to_json(JSon& j, const ServerStatus& s) ;
    void from_json(const JSon& j, ServerStatus& s);

} // DssApi

#endif //DSS_IF_SERVERSTATUS_H
