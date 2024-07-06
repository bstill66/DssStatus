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
                INT_AVL = 7,
                PR_AVL = 6,
                TV_AVL = 5
            } Name;

            void clear();

            void setAvailability(Name id, bool state);
            bool getAvailability(Name id) const;

            void setUpTime(uint16_t ut) { upTime = ut;}
            uint16_t getUpTime() const {return upTime;}

            void setPairingCount(uint16_t cnt) { pairingCount = cnt;}
            uint16_t getPairingCount() const {return pairingCount;}

            size_t  append(ByteBuffer& buf) const;

            ByteBuffer::const_iterator& loadBinary(ByteBuffer::const_iterator & it);
            void randomize();

            bool operator==(const ServerStatus& rhs) const;
            bool operator!=(const ServerStatus& rhs) const {return !(*this == rhs);}
        protected:
            uint8_t       availability;
            uint8_t       upTime;
            uint8_t       pairingCount;
            uint8_t       reserved;

    };

} // DssApi

#endif //DSS_IF_SERVERSTATUS_H
