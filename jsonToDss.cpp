//
// Created by bstil on 11/18/2024.
//

#include <iostream>
#include <argparse/argparse.hpp>
#include <fstream>
#include "DssStatus.h"





static argparse::ArgumentParser& parseArgs(int argc,char *argv[]) {
    using namespace argparse;
    using namespace std;

    static ArgumentParser parser(argv[0]);

    parser.add_argument("-o","--output")
          .nargs(1)
          .default_value("dss.b64");

    parser.add_argument("inpFile")
            .nargs(1)
            .required()
            .help("Input file (JSON format)");

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
        DssStatus dssInfo(infile);

        std::string bin = dssInfo.asBase64();

        std::cout << bin << std::endl;
        outfile << bin;

    } catch (const nlohmann::detail::parse_error& e) {
        const std::string& msg = e.what();
        int ndx = msg.find("]");
        std::cerr << msg.substr(ndx+1) << std::endl;
        return 1;
    }




    return 0;
}
