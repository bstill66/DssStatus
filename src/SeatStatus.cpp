//
// Created by bstil on 7/1/2024.
//

#include "base64pp/base64pp.h"
#include "SeatStatus.h"
#include "JsonUtils.h"


namespace DssApi {
    /**
     * Default constructor
     */
    SeatStatus::SeatStatus()
    :  status(0), mode(0)
{
    clear();
}

void SeatStatus::clear() {
    status  = 0;
    mode    = 0;

    // set UI state to UNSPECIFIED
    setUIState(UNSPECIFIED);
}


uint8_t SeatStatus::getStatus(SeatStatus::Availability st) const {
    // pull the binary value from the status field based on the availability setting
    return getField(status,1,st);
}

void SeatStatus::setStatus(SeatStatus::Availability st, int val) {
    // update the status field to the specified value
    status = setField(status,1,st,val);
}

uint8_t SeatStatus::getMode(Mode m) const {
    return getField(mode,1,m);
}

uint8_t SeatStatus::setMode(Mode m,uint8_t val) {
    return setField(mode,1,m,val);
}

void SeatStatus::setUIState(DssApi::SeatStatus::UIState s) {
    setField(mode,4,0,s);
}

SeatStatus::UIState
SeatStatus::getUIState() const {
    return static_cast<UIState>(getField(mode,4,0));
}

size_t SeatStatus::write(ByteBuffer &buf) const {
    size_t start = buf.size();

    buf.push_back(status);
    buf.push_back(mode);

    return buf.size()-start;
}


ByteBuffer::const_iterator&
SeatStatus::read(ByteBuffer::const_iterator& it) {

    status  = *it++;
    mode    = *it++;

    return it;
}

bool SeatStatus::operator==(const SeatStatus &rhs) const {
    // Standard equality checks
    return (status      == rhs.status) &&
           (mode        == rhs.mode);
}


    void to_json(JSon& j, const SeatStatus& s) {
        using namespace DssApi;

        j = {
             {"DSS_COMM_LOSS",s.getStatus(SeatStatus::DSS_COMM_LOSS)},
             {"TM_SYNC",      s.getStatus(SeatStatus::TM_SYNC)},
             {"TV_SVC_AVL",   s.getStatus(SeatStatus::TV_SVC_AVL)},
             {"VLS",          s.getStatus(SeatStatus::VLS)},
             {"PA",           s.getStatus(SeatStatus::PA)},
             {"PCTL_LOCK",    s.getStatus(SeatStatus::PCTL_LOCK)},
             {"STOWD",        s.getStatus(SeatStatus::STOWD)},

             {"KID",          s.getMode(SeatStatus::KID)},
             {"LOGIN_AVL",    s.getMode(SeatStatus::LOGIN_AVL)},
             {"LOGD_IN",      s.getMode(SeatStatus::LOGD_IN)},
             {"UI",           s.getUIState()}};

    }

    void from_json(const JSon& j, SeatStatus& s) {
        using namespace JsonUtils;

        s.clear();

        // Retrieve all the status objects
        s.setStatus(SeatStatus::DSS_COMM_LOSS,getBoolean(j,"DSS_COMM_LOSS"));
        s.setStatus(SeatStatus::TM_SYNC,getBoolean(j,"TM_SYNC"));
        s.setStatus(SeatStatus::TV_SVC_AVL,getBoolean(j,"TV_SVC_AVL"));
        s.setStatus(SeatStatus::VLS,getBoolean(j,"VLS"));
        s.setStatus(SeatStatus::PA,getBoolean(j,"PA"));
        s.setStatus(SeatStatus::PCTL_LOCK,getBoolean(j,"PCTL_LOCK"));
        s.setStatus(SeatStatus::STOWD,getBoolean(j,"STOWD"));

        // and the mode objects...
        s.setMode(SeatStatus::LOGIN_AVL,getBoolean(j,"LOGIN_AVL"));
        s.setMode(SeatStatus::LOGD_IN,getBoolean(j,"LOGD_IN"));
        s.setMode(SeatStatus::KID,getBoolean(j,"KID"));

        // and the UI state
        s.setUIState(static_cast<SeatStatus::UIState>(j["UI"]));

    }
} // DssApi