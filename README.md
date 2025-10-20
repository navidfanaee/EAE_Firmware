# EAE_Firmware 

## Overview
This project simulates a cooling-controller firmware communicating over Linux Virtual CAN (`vcan0`).  


## How to Build
```bash
sudo apt install build-essential cmake can-utils
./scripts/start_vcan.sh          # create vcan0
mkdir build && cd build
cmake .. && cmake --build . -j
ctest --output-on-failure        # run all tests


