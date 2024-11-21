# DSS Status Simulator and Test tool(s)
## Docker 
A primitive server can be easily built that emulates the DSS interface in accordance with the API and ICD documentation. By adjusting command line
arguments to the test environment, you can adjust some aspects as integration progresses.

All of these utilities require the installation of Docker on the host platform.
### Build
Using the provided `Dockerfile`, simply run the following commands with the provided `Dockerfile` in the current directory.
```
docker build -t <tag> .
```
After about 1 minute, the application code is installed, built and tested.

### Run/Test
Once the Docker image has been built, it can easily be run. By default, it emulates the intended final configuration. However, parameters can be modified 
to suit integration and test needs. Among these are:
| Parameter | Description |
| ----------- | ----------- |
| --url | Alternate URL endpoint |
| -r | Specify number of seats to generate random data for|
| --user | Specify the user name used in Authentication |
| --password | Specify alternate password used for Authentication |

To run the emulator, simply perform the following operation:
```
docker run -t <tag> --name <name> -p<local port>:80 <optional parameters from above>
```

To verify proper operation, you can test it using `curl`, as follows:
```
curl --basic -u <user>:<password> localhost:<local port>/qoe/snapshot
```
## Native/Host Environment
The tool can be built/ran from a local host.
### Build
In order to build the code natively, you will need to have a C++ 20 compiler tool chain
and CMAKE, GIT and native make installed. On a Linux based platform, perform the following:
```angular2html
apt install build-essential cmake make git
```

Clone the source/build tree from github.
```angular2html
git clone https://github.com/bstill66/DssStatus.git
```

And build it
```angular2html
cmake -B <build-dir> -S <source dir>
```
The resulting executables will be in specified <source dir>

### Run
This section provides details on running the various test tools.
#### DSS Emulating Server
This application emulates the DSS server, providing methods for 
overriding key aspects of the server to aid in integration. These options
can be obtained by invoking the help functionality of the application
as follows: `./DssServer.exe --help`

```angular2html


Usage: DssServer.exe [--help] [--version] [--port VAR] [--random VAR] [--user
VAR] [--password VAR] [--url VAR] inpFile

Positional arguments:
inpFile        Input file (JSON format) [nargs=0..1] [default: ""]

Optional arguments:
-h, --help     shows help message and exits
-v, --version  prints version information and exits
-p, --port     [nargs=0..1] [default: 8080]
-r, --random   [nargs=0..1] [default: 20]
-u, --user     User for Authorization [nargs=0..1] [default: ]
--password     User Password for Authorization [nargs=0..1] [default: ]
--url          URL Endpoint for service [nargs=0..1] [default: "/qoe/snapshot"]
```
#### fromJson
This utility reads a JSON formatted file (based on the ICD) and generates the
Base64 output. `./fromJson <input file>`.

By default, the output is written to a file called "dss.b64" in the current directory
but can be over-ridden with the -o option.
```angular2html
./fromJson.exe testdata\test.json
RGgAIgEAVMQXUKon
```
#### toJson
This utility is the opposite of the fromJson tool. It takes as input a file
containing the Base64 representation of a response and converts it into a JSON object
and writes it to "dss.json"; but can also be over-ridden using the -o option.
```angular2html
./toJson -o <outfile> <input file>
cat <outfile>
```
