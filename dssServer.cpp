//
// Created by bstil on 11/19/2024.
//
#include <iostream>
#include <argparse/argparse.hpp>
#include <fstream>
#include <format>
#include "DssStatus.h"
#include "SeatStatus.h"
#include "httplib.h"

using namespace DssApi;

static argparse::ArgumentParser& parseArgs(int argc,char *argv[]) {
    using namespace argparse;
    using namespace std;

    static ArgumentParser parser(argv[0]);

    parser.add_argument("-p","--port")
            .nargs(1)
            .default_value(8080)
            .scan<'i',int>();

    parser.add_argument("-r","--random")
            .nargs(1)
            .default_value(20)
            .scan<'i',int>();

    parser.add_argument("inpFile")
            .nargs(1)
            .required()
            .help("Input file (JSON format)");

    parser.parse_args(argc,argv);

    return parser;
}

std::string readData(const std::string& fname) {
    using namespace DssApi;

    std::ifstream   infile(fname);

    if (!infile.good()) {
        return "Unable to open " + fname;
    }

    try {
        DssStatus dssInfo(infile);

        std::string bin = dssInfo.asBase64();

        return bin;

    } catch (const nlohmann::detail::parse_error& e) {
        const std::string& msg = e.what();
        int ndx = msg.find("]");
        std::cerr << msg.substr(ndx+1) << std::endl;
        return msg.substr(ndx+1);
    }

}

static
std::string getRandomSeatId() {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<uint8_t> rowDistro(1,63);
    static std::uniform_int_distribution<uint8_t> colDistro('A','P');

    uint16_t row = rowDistro(generator);
    uint16_t col = colDistro(generator);
    std::string seatId = std::format("{:d}{:c}",row,col);

    return seatId;
}

static
SeatStatus loadRandomSeat() {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<uint8_t> counter(0,255);
    static std::uniform_int_distribution<uint8_t> binary(0,1);
    static std::uniform_int_distribution<uint8_t> uiState(0,15);


    SeatStatus   tmp;
    tmp.setStatus(DssApi::SeatStatus::DSS_COMM_LOSS, binary(generator));
    tmp.setStatus(DssApi::SeatStatus::TV_SVC_AVL, binary(generator));
    tmp.setStatus(DssApi::SeatStatus::VLS,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::TM_SYNC,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::PCTL_LOCK,binary(generator));
    tmp.setStatus(DssApi::SeatStatus::PA,binary(generator));
    tmp.setStatus(SeatStatus::STOWD,binary(generator));

    tmp.setMode(DssApi::SeatStatus::Mode::LOGIN_AVL,binary(generator));
    tmp.setMode(DssApi::SeatStatus::Mode::LOGD_IN,binary(generator));
    tmp.setMode(DssApi::SeatStatus::Mode::KID,binary(generator));
    tmp.setUIState(static_cast<DssApi::SeatStatus::UIState>(uiState(generator)));

    return tmp;
}

static
DssStatus loadRandomSeats(int numSeats) {
    using namespace std;

    DssStatus  dss;
    set<string>   usedIds;

    while (numSeats != 0) {
        string id = getRandomSeatId();
        if (usedIds.contains(id) == false) {

            SeatStatus tmp = loadRandomSeat();
            dss.add(id,tmp);

            usedIds.insert(id);
            numSeats -= 1;
        }
    }

    return dss;
}

int main(int argc,char *argv[]) {

    using namespace DssApi;
    using namespace argparse;

    ArgumentParser& params = parseArgs(argc,argv);

    const std::string& ifname = params.get<std::string>("inpFile");
    int port = params.get<int>("--port");
    int rnd  = params.get<int>("--random");

    // HTTP
    httplib::Server svr;

    if (rnd > 0) {
        svr.Get("/dss",[&rnd](const httplib::Request &, httplib::Response &res) {
                   DssStatus  tmp = loadRandomSeats(rnd);
                   std::string bin = tmp.asBase64();
                   res.set_content(bin, "text/plain");
                });
    } else {
        svr.Get("/dss", [&ifname](const httplib::Request &, httplib::Response &res) {
                        res.set_content(readData(ifname), "text/plain");
                        });
    }

    svr.listen("0.0.0.0", port);

    std::ifstream   infile(ifname);

    if (!infile.good()) {
        std::cerr << "Unable to open " << ifname << std::endl;
        return 1;
    }



    return 0;
}
