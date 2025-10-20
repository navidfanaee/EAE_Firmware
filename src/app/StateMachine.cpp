#include "app/StateMachine.hpp"
#include "../DebugMonitor.hpp"
#include <string>

using namespace eae::app;

StateMachine::StateMachine()
: state_(SystemState::OFF), state_entry_time_ms_(0), boost_remaining_ms_(0), on_transition_(nullptr) {}
// handle state transitions and associated actions
void StateMachine::enter_state(SystemState s, uint64_t now_ms) {
    SystemState prev = state_;
    state_ = s;
    state_entry_time_ms_ = now_ms;
    if (s == SystemState::BOOST_COOLING) boost_remaining_ms_ = 30000; // default boost duration
    if (on_transition_) on_transition_(prev, s, now_ms);
}
// get state name
std::string StateMachine::state_name() const {
    switch (state_) {
        case SystemState::OFF: return "OFF";
        case SystemState::STARTUP_PRIME: return "STARTUP_PRIME";
        case SystemState::IDLE: return "IDLE";
        case SystemState::ACTIVE_COOLING: return "ACTIVE_COOLING";
        case SystemState::BOOST_COOLING: return "BOOST_COOLING";
        case SystemState::DERATE: return "DERATE";
        case SystemState::FAULT: return "FAULT";
        case SystemState::MAINTENANCE: return "MAINTENANCE";
    }
    return "UNKNOWN";
}
// step the state machine based on inputs and produce outputs
SystemOutputs StateMachine::step(const SystemInputs &in) {
    SystemOutputs out{};
    out.state = state_;

    // Safety override: immediate FAULT if level missing
    if (!in.level_ok && state_ != SystemState::FAULT) {
        enter_state(SystemState::FAULT, in.now_ms);
    }

    // immediate critical temp -> BOOST_COOLING
    if (in.temperature_c >= 95.0 && state_ != SystemState::BOOST_COOLING) {
        enter_state(SystemState::BOOST_COOLING, in.now_ms);
    }

    switch (state_) {
        case SystemState::OFF:
            out.inverter_derate = true;
            if (in.ignition) enter_state(SystemState::STARTUP_PRIME, in.now_ms);
            break;
        case SystemState::STARTUP_PRIME:
            out.pump_cmd = true;
            out.inverter_derate = true;
            // require pump feedback to go to IDLE; if missing then FAULT after prime duration
            if (!in.level_ok) enter_state(SystemState::FAULT, in.now_ms);
            if (in.pump_feedback_ok) enter_state(SystemState::IDLE, in.now_ms);
            break;
        case SystemState::IDLE:
            out.pump_cmd = true;
            out.inverter_derate = false;
            if (!in.ignition) enter_state(SystemState::OFF, in.now_ms);
            if (in.remote_derate_request) enter_state(SystemState::DERATE, in.now_ms);
            if (in.temperature_c >= 67.0) enter_state(SystemState::ACTIVE_COOLING, in.now_ms);
            break;
        case SystemState::ACTIVE_COOLING:
            out.pump_cmd = true;
            out.fan_pwm = 50; // suggestion; PID will refine
            out.inverter_derate = false;
            if (!in.ignition) enter_state(SystemState::OFF, in.now_ms);
            if (!in.level_ok) enter_state(SystemState::FAULT, in.now_ms);
            if (in.temperature_c >= 75.0) enter_state(SystemState::DERATE, in.now_ms);
            if (in.temperature_c <= 65.0) enter_state(SystemState::IDLE, in.now_ms);
            break;
        case SystemState::BOOST_COOLING:
            out.pump_cmd = true;
            out.fan_pwm = 100;
            out.inverter_derate = true;
            if (!in.level_ok) enter_state(SystemState::FAULT, in.now_ms);
            // boost handled externally by decrementing boost_remaining_ms_
            if (boost_remaining_ms_ <= 0) {
                if (in.temperature_c >= 75.0) enter_state(SystemState::DERATE, in.now_ms);
                else if (in.temperature_c <= 65.0) enter_state(SystemState::IDLE, in.now_ms);
                else enter_state(SystemState::ACTIVE_COOLING, in.now_ms);
            }
            break;
        case SystemState::DERATE:
            out.pump_cmd = true;
            out.fan_pwm = 100;
            out.inverter_derate = true;
            if (!in.level_ok) enter_state(SystemState::FAULT, in.now_ms);
            if (in.temperature_c <= 65.0 && !in.remote_derate_request) enter_state(SystemState::IDLE, in.now_ms);
            break;
        case SystemState::FAULT:
            out.pump_cmd = false;
            out.fan_pwm = 100;
            out.inverter_derate = true;
            out.alarm = true;
            if (in.manual_reset && in.level_ok && in.pump_feedback_ok) enter_state(SystemState::STARTUP_PRIME, in.now_ms);
            break;
        case SystemState::MAINTENANCE:
            out.pump_cmd = false;
            out.fan_pwm = 0;
            break;
    }

    out.state = state_;
    return out;
}
