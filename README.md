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
docker run -t <tag> --name <name>  <optional parameters from above>
```
