//
// Created by bstil on 7/1/2024.
//

#ifndef DSS_IF_DSSSTATUS_H
#define DSS_IF_DSSSTATUS_H

#include <string>
#include <map>
#include <memory>

#include "DssCmn.h"
#include "ServerStatus.h"
#include "SeatStatus.h"




namespace DssApi {

class DssStatus {

public:
    typedef std::map <std::string,SeatStatus> SeatCollection;

    DssStatus();

    DssStatus(const std::string &);

    DssStatus(std::istream &infile);

    void clear();
    std::string asBase64() const;

    void set(const ServerStatus& s);
    const ServerStatus& getServerStatus() const;


    void add(const std::string& id,const SeatStatus& s);
    const SeatCollection& getSeatStatus() const;

    bool fromBase64(const std::string &b64);


    bool operator==(const DssStatus &rhs) const;

    size_t numSeats() const { return seats.size();}

protected:


private:
    ServerStatus             server;
    SeatCollection           seats;

};

    void to_json(JSon& j, const DssStatus& s) ;
    void from_json(const JSon& j, DssStatus& s);
}
#endif //DSS_IF_XXX_H
