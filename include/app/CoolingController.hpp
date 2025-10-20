#pragma once
#include "drivers/CANParser.hpp"
#include "app/StateMachine.hpp"
#include "app/PID.hpp"

namespace eae::app {

class CoolingController {
public:
    CoolingController();
    void update(const drivers::CANParser& can, uint64_t now_ms);
    const SystemOutputs& outputs() const { return out_; }

private:
    StateMachine sm_;
    PID pid_;
    SystemOutputs out_;
    double setpoint_;
};

} // namespace eae::app