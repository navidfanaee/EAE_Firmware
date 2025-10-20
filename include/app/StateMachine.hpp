#pragma once
#include <cstdint>
#include <string>
#include <functional>

namespace eae::app {
// defining various system states for the state machine
enum class SystemState : uint8_t {
    OFF,
    STARTUP_PRIME,
    IDLE,
    ACTIVE_COOLING,
    BOOST_COOLING,
    DERATE,
    FAULT,
    MAINTENANCE
};
// defining system input structure to hold various sensor and command inputs
struct SystemInputs {
    double temperature_c{25.0};
    bool ignition{false};
    bool level_ok{true};
    bool pump_feedback_ok{true};
    bool derate_request{false};
    bool manual_reset{false};
    uint64_t now_ms{0};
};
// defining system output structure to hold various command outputs
struct SystemOutputs {
    bool pump_cmd{false};
    int fan_pwm{0}; // 0-100
    bool inverter_derate{false};
    bool alarm{false};
    SystemState state{SystemState::OFF};
};
// defining transition callback type for state changes
using TransitionCallback = std::function<void(SystemState from, SystemState to, uint64_t now_ms)>;
// State machine class to manage system states based on inputs and produce outputs
class StateMachine {
public:
    StateMachine();

    // step the state machine with current inputs; returns outputs (pump, fan suggestion, alarm...)
    SystemOutputs step(const SystemInputs &in);

    SystemState state() const { return state_; }
    std::string state_name() const;

    // Optional: register a callback which is invoked whenever state changes
    void set_transition_callback(TransitionCallback cb) { on_transition_ = std::move(cb); }

private:
    void enter_state(SystemState s, uint64_t now_ms);
    SystemState state_;
    uint64_t state_entry_time_ms_;
    int boost_remaining_ms_;
    TransitionCallback on_transition_;
};

} 
