//
// Created by bstil on 11/18/2024.
//

#include <iostream>
#include <argparse/argparse.hpp>
#include <fstream>
#include "DssStatus.h"
#include "JsonUtils.h"

class PgmOptions {
public:
    typedef enum {
        NO_ACTION,
        ENCODE_STRING,
        DECODE_STRING,
    } Mode;

    PgmOptions() {random=-1;}
    Mode   mode;

    bool        encodeFlag;
    std::string inputString;
    std::string infile;
    std::string outfile;
    int         random;
};




static argparse::ArgumentParser& parseArgs(int argc,char *argv[]) {
    using namespace argparse;
    using namespace std;

    static ArgumentParser parser(argv[0]);

    parser.add_argument("-o","--output")
          .nargs(1)
          .default_value("out.json");

    parser.add_argument("inpFile")
            .nargs(1)
            .required()
            .help("Input file (DSS/Base64 format)");

    parser.parse_args(argc,argv);

    return parser;
}


int main(int argc,char *argv[]) {

    using namespace DssApi;
    using namespace argparse;

    ArgumentParser& params = parseArgs(argc,argv);

    const std::string& ifname = params.get<std::string>("inpFile");
    const std::string& ofname = params.get<std::string>("--output");

    std::filesystem::path cwd = std::filesystem::current_path();
    std::ifstream   infile(ifname);
    if (!infile.good()) {
        std::cerr << "Unable to open " << ifname << std::endl;
        return 1;
    }

    std::ofstream    outfile(ofname);
    if (!outfile.good()) {
        std::cerr << "Unable to open " << ofname << std::endl;
        return 1;
    }

    try {
        std::string line;
        std::string buffer;
        while (std::getline(infile, line)) {
            buffer += line;
        }

        DssStatus dssInfo;
        dssInfo.fromBase64(buffer);

        JSon  obj;
        buffer = JsonUtils::toJsonString(dssInfo);

        outfile.write(buffer.c_str(),buffer.length());

    } catch (const nlohmann::detail::parse_error& e) {
        const std::string& msg = e.what();
        int ndx = msg.find("]");
        std::cerr << msg.substr(ndx+1) << std::endl;
        return 1;
    }




    return 0;
}
