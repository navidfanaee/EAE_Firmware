#include "app/CoolingController.hpp"

using namespace eae::app;
using namespace eae::drivers;

CoolingController::CoolingController()
: pid_(1.5, 0.2, 0.05, 0.0, 100.0),
  setpoint_(70.0) {}

void CoolingController::update(const CANParser& can, uint64_t now_ms) {
    SystemInputs in{};
    in.temperature_c = can.temperature();
    in.ignition = can.ignition();
    in.level_ok = can.level_ok();
    in.pump_feedback_ok = can.pump_feedback_ok();
    in.derate_request = false;
    in.manual_reset = can.manual_reset();
    in.now_ms = now_ms;

    auto out = sm_.step(in);

    // adjust fan based on PID using temperature error
    double fan_pwm = pid_.update(setpoint_, in.temperature_c, 0.1);
    out.fan_pwm = static_cast<int>(fan_pwm);
    out_ = out;
}