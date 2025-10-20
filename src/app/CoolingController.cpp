#include "app/CoolingController.hpp"
#include "../Config.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace eae::app;
using namespace eae::drivers;
using namespace eae::config;

CoolingController::CoolingController(CANParser &parser, ActuatorsSim &actuators, DebugMonitor &dbg)
: can_(parser),
  actuators_(actuators),
  dbg_(dbg),
  pid_(1.2, 0.05, 0.02, 0.0, 100.0),
  watchdog_(WATCHDOG_TIMEOUT_MS),
  setpoint_c_(DEFAULT_SETPOINT_C),
  last_sensor_ts_ms_(0),
  pump_commanded_(false) 
{
    // print state transitions to console
    fsm_.set_transition_callback([this](SystemState from, SystemState to, uint64_t now_ms){
        std::cout << "[TRANS] " << static_cast<int>(from) << " -> " << static_cast<int>(to) << " @ " << now_ms << " (" << fsm_.state_name() << ")\n";
    });
}

bool CoolingController::init() {
    // nothing to init for parser/actuators in this test harness
    return true;
}

void CoolingController::apply_outputs(const StateMachine::SystemOutputs &o) {
    // actuators: fan and pump duty based on outputs
    int fan_pwm = o.fan_pwm;
    int pump_duty = o.pump_cmd ? 80 : 0;
    actuators_.set_fan_pwm(fan_pwm);
    actuators_.set_pump_duty(pump_duty);
    pump_commanded_ = o.pump_cmd;
}

void CoolingController::tick(double dt_sec, uint64_t now_ms) {
    // read latest parsed sensor data
    auto sd = can_.latest();
    last_sensor_ts_ms_ = now_ms; // timestamping for this model

    // Build inputs
    SystemInputs in;
    in.temperature_c = sd.temperature_c;
    in.ignition = true; // for demo we assume ignition on; can be changed via CAN
    in.level_ok = sd.level_ok;
    in.pump_feedback_ok = sd.pump_running;
    in.now_ms = now_ms;
    in.remote_derate_request = false;
    in.manual_reset = false;

    // Safety checks (basic)
    bool fault = false;
    if ((now_ms - last_sensor_ts_ms_) > SENSOR_TIMEOUT_MS) {
        dbg_.note_fault("sensor_timeout");
        fault = true;
    }
    if (!sd.level_ok) {
        dbg_.note_fault("level_low");
        fault = true;
    }
    if (fault) in.level_ok = false; // force state machine fault

    // Step state machine
    auto outs = fsm_.step(in);

    // PID runs only in ACTIVE_COOLING to set fan PWM more smoothly
    if (outs.state == SystemState::ACTIVE_COOLING) {
        double pid_out = pid_.update(setpoint_c_, sd.temperature_c, dt_sec);
        outs.fan_pwm = static_cast<int>(pid_out);
    }

    // apply outputs to actuators
    apply_outputs(outs);
    outs_ = outs;

    // publish debug CAN status? (not implemented here, but we print)
    std::cout << std::fixed << std::setprecision(2)
        << "[CTRL] T=" << sd.temperature_c << "C SP=" << setpoint_c_
        << " | State=" << fsm_.state_name()
        << " | Fan=" << outs.fan_pwm << "% PumpCmd=" << (outs.pump_cmd ? "ON" : "OFF") << "\n";

    // pet watchdog
    watchdog_.pet(now_ms);
    dbg_.tick(now_ms);
}
