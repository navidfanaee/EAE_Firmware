# EAE_Firmware — Cooling System Control Loop Simulation

## Overview
EAE_Firmware is a C++17 simulation of an embedded cooling-system firmware.  
It models the real-time control loop found in automotive and industrial systems:
- Receives sensor data via virtual CAN bus simulator(`vcan0` on Linux)
- Parses messages into engineering values (`CANParser`)
- Evaluates system logic through a finite-state machine
- Applies a PID controller to regulate fan speed
- Outputs real-time system status to the console


## System Architecture

### Components
| Module | Responsibility |
|---------|----------------|
| CANParser | Reads CAN frames and extracts temperature, ignition, pump feedback, and coolant level. |
| StateMachine | Implements system operating states and transitions (OFF → STARTUP → IDLE → COOLING → BOOST → DERATE → FAULT). |
| PID | Calculates proportional–integral–derivative fan control output. |
| CoolingController | Integrates parser + state machine + PID into a single control loop. |
| main.cpp | Runs the firmware loop, connects to `vcan0`, and prints live state transitions. |

---

## System States

| State | Description | Actions |
|--------|--------------|---------|
| OFF | System idle, ignition off | Derate active, all actuators off |
| STARTUP_PRIME | Pump priming phase | Pump ON, derate active |
| IDLE | Ready, temperature low | Pump ON, fan OFF |
| ACTIVE_COOLING | Normal cooling | Pump ON, fan controlled by PID |
| BOOST_COOLING | Emergency cooling (≥95 °C) | Pump ON, fan = 100 %, derate active |
| DERATE | Power derate requested | Pump + fan = 100 %, derate active |
| FAULT | Coolant level or pump fault | Alarm ON, pump OFF |
| MAINTENANCE | Manual maintenance mode | All actuators OFF |

---
## Build Instructions

### 🔧 Prerequisites
Ubuntu 20.04 + (or similar)  
Packages:
```bash
sudo apt install build-essential cmake can-utils

Build Steps

git clone https://github.com/navidfanaee/EAE_Firmware.git
cd EAE_Firmware
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

Executables will appear in the build/ directory.

Run Unit Tests

cd build
ctest --output-on-failure --verbose

You should see:

100% tests passed, 0 tests failed

Run the Firmware Simulation
1- Create a virtual CAN interface

./scripts/start_vcan.sh

2- Start the firmware (with optional temperature setpoint)

./build/eae_firmware 75

If no argument is given, the default setpoint is 70 °C.
3- In another terminal, send simulated CAN data with an argument determining can data rate 

./scripts/send_can.sh 5

4- Observe the live output

[INFO] Connected to vcan0
[INFO] Using temperature setpoint: 75°C
[CTRL] State=STARTUP_PRIME  T=25°C  Fan=0%  Pump=ON  Derate=Y
[CTRL] State=IDLE           T=68°C  Fan=0%  Pump=ON  Derate=N
[CTRL] State=ACTIVE_COOLING T=80°C  Fan=47% Pump=ON  Derate=N
[CTRL] State=BOOST_COOLING  T=95°C  Fan=100% Pump=ON Derate=Y
[CTRL] State=IDLE           T=65°C  Fan=0%  Pump=ON  Derate=N

Stop with Ctrl + C.

Design Notes

Timing

CAN frames arrive every ~200 ms (5HZ)(send_can.sh).

Control loop runs at 20 Hz (50 ms period) → faster than CAN traffic for responsive transitions.

Safety & Fault Logic

Missing level signal → immediate FAULT.

Temperature ≥ 95 °C → BOOST_COOLING.



Example Console Session
$ ./scripts/start_vcan.sh
[INFO] vcan0 created and ready
$ ./build/eae_firmware 75
[INFO] Connected to vcan0
[INFO] Using temperature setpoint: 75°C
[CTRL] State=IDLE           T=70°C  Fan=10% Pump=ON  Derate=N
[CTRL] State=ACTIVE_COOLING T=82°C  Fan=42% Pump=ON  Derate=N
[CTRL] State=BOOST_COOLING  T=96°C  Fan=100% Pump=ON Derate=Y
