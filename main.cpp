#include <iostream>
#include <argparse/argparse.hpp>
#include <fstream>
#include "DssStatus.h"

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




static PgmOptions parseArgs(int argc,char *argv[]) {
    using namespace argparse;


    ArgumentParser parser(argv[0]);

    parser.add_argument("-e").flag();
    parser.add_argument("-i").nargs(1);
    parser.add_argument("-I").nargs(1);
    parser.add_argument("-o").nargs(1);
    parser.add_argument("-R").nargs(1);

    parser.parse_args(argc,argv);

    PgmOptions  opt;
    opt.mode = PgmOptions::NO_ACTION;

    if (parser.present("-e")) {
        opt.inputString = parser.get<std::string>("-e");
        opt.mode = PgmOptions::ENCODE_STRING;
    }
    else if (parser.present("-d")) {
        opt.inputString = parser.get<std::string>("-d");
        opt.mode = PgmOptions::DECODE_STRING;
    }

    opt.outfile    = parser.get<std::string>("-o");
    opt.infile     = parser.get<std::string>("-i");
    auto tmp       = parser.get<std::string>("-R");
    opt.random     = atoi(tmp.c_str());
    return opt;
}


int main(int argc,char *argv[]) {

    using namespace DssApi;

  PgmOptions params = parseArgs(argc,argv);

  DssStatus  *dssStatus = nullptr;

  std::ifstream   infile(params.infile);
  if (!infile.good()) {
     std::cerr << "Unable to open " << params.infile << std::endl;
     return 1;
  }
  else {
      dssStatus = new DssStatus(infile);


      std::string ascii = dssStatus->asBase64();

      std::cout << "Encoding --> " << ascii << std::endl;
  }

  if (params.random != -1) {
      dssStatus = new DssStatus();


    //ssStatus->randomize(params.random);

      std::string ascii = dssStatus->asBase64();
      std::cout << "Randomized length w/ " << params.random << " seats = " << ascii.length() << std::endl;

      DssStatus  tmp;
      tmp.fromBase64(ascii);


  }

  if (dssStatus != nullptr) {
      std::string ascii = dssStatus->asBase64();

      std::ofstream  ofs(params.outfile);
      ofs << ascii;
  }

  return 0;
}
