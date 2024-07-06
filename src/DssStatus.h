//
// Created by bstil on 7/1/2024.
//

#ifndef DSS_IF_DSSSTATUS_H
#define DSS_IF_DSSSTATUS_H

#include <string>
#include <list>
#include <memory>

#include "DssCmn.h"
#include "ServerStatus.h"
#include "SeatStatus.h"




namespace DssApi {

class DssStatus {

public:

    DssStatus();

    DssStatus(const std::string &);

    DssStatus(std::istream &infile);

    std::string asBase64() const;

    bool fromBase64(const std::string &b64);

    void randomize(int numSeats);

    bool operator==(const DssStatus &rhs) const;

    size_t numSeats() const { return seats.size();}

protected:
    typedef std::list <SeatStatus> SeatCollection;

private:
    ServerStatus             dssStatus;
    SeatCollection           seats;

};
}
#endif //DSS_IF_XXX_H
