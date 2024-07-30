//
// Created by bstil on 7/28/2024.
//

#include <random>
#include <format>
#include <set>

#include "ServerStatus.h"
#include "SeatStatus.h"
#include "DssStatus.h"


using namespace DssApi;

static std::default_random_engine generator;
static std::uniform_int_distribution<uint8_t> counter(0,255);
static std::uniform_int_distribution<uint8_t> binary(0,1);
static std::uniform_int_distribution<uint8_t> rowDistro(1,63);
static std::uniform_int_distribution<uint8_t> colDistro('A','P');
static std::uniform_int_distribution<uint8_t> uiState(0,15);


ServerStatus loadRandomStatus() {
    ServerStatus  tmp;

    tmp.setUpTime(counter(generator));

    tmp.setAvailability(DssApi::ServerStatus::PR_AVL,binary(generator));
    tmp.setAvailability(DssApi::ServerStatus::INT_AVL,binary(generator));
    tmp.setAvailability(DssApi::ServerStatus::TV_AVL,binary(generator));
    tmp.setAvailability(DssApi::ServerStatus::FLT_OPN,binary(generator));
    tmp.setAvailability(DssApi::ServerStatus::LPNS_AVL,binary(generator));

    return tmp;
}



SeatStatus loadRandomSeat() {
    uint16_t row = rowDistro(generator);
    uint16_t col = colDistro(generator);
    auto seatId = std::format("{:d}{:c}",row,col);

    SeatStatus   tmp(seatId);
    tmp.setStatus(DssApi::SeatStatus::DSS_COMM_LOSS, binary(generator));
    tmp.setStatus(DssApi::SeatStatus::TV_SVC_AVL, binary(generator));
    tmp.setStatus(DssApi::SeatStatus::VLS,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::TM_SYNC,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::PCTL_LOCK,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::PA,binary(generator));

    tmp.setMode(DssApi::SeatStatus::Mode::LOGIN_AVL,binary(generator));
    tmp.setMode(DssApi::SeatStatus::Mode::LOGD_IN,binary(generator));
    tmp.setMode(DssApi::SeatStatus::Mode::KID,binary(generator));
    tmp.setUIState(static_cast<DssApi::SeatStatus::UIState>(uiState(generator)));

    return tmp;
}

DssStatus loadRandomDss(int numSeats) {
    using namespace std;

    DssStatus dss;
    set<string>    usedIds;  // used to prevent duplicates

    dss.set(loadRandomStatus());

    while (numSeats != 0) {
        SeatStatus  seat = loadRandomSeat();

        string id = seat.getSeatId();
        if (usedIds.contains(id) == false) {
            dss.add(seat);
            numSeats -= 1;
        }
    }

    return dss;
}