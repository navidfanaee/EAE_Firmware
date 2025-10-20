#pragma once
#include "../drivers/CANParser.hpp"
#include "../drivers/ActuatorsSim.hpp"
#include "../utils/Watchdog.hpp"
#include "PID.hpp"
#include "StateMachine.hpp"
#include "../DebugMonitor.hpp"
#include <atomic>

namespace eae::app {
// Cooling controller class to manage cooling system based on sensor inputs and state machine
class CoolingController {
public:
    CoolingController(eae::drivers::CANParser &parser,
                      eae::drivers::ActuatorsSim &actuators,
                      eae::DebugMonitor &dbg);

    // init (opens drivers as needed). Returns true if OK.
    bool init();

    // Main tick called periodically: dt_sec seconds and now_ms timestamp
    void tick(double dt_sec, uint64_t now_ms);

    // expose last outputs for testing/inspection
    StateMachine::SystemOutputs last_outputs() const { return outs_; }
    SystemState last_state() const { return fsm_.state(); }

private:
    eae::drivers::CANParser &can_;
    eae::drivers::ActuatorsSim &actuators_;
    eae::DebugMonitor &dbg_;
    PID pid_;
    StateMachine fsm_;
    eae::utils::Watchdog watchdog_;
    double setpoint_c_;
    uint64_t last_sensor_ts_ms_;
    bool pump_commanded_;
    StateMachine::SystemOutputs outs_;

    // helper to publish/actuate
    void apply_outputs(const StateMachine::SystemOutputs &o);
};
