#pragma once
#include "drivers/CANParser.hpp"
#include "app/StateMachine.hpp"
#include "app/PID.hpp"
#include <string>

namespace eae::app {

class CoolingController {
public:
    CoolingController();
    std::string state_name() const;   // declaration only
    void update(const drivers::CANParser& can, uint64_t now_ms);
    const SystemOutputs& outputs() const { return out_; }
    void setSetpoint(double sp) { setpoint_ = sp; } 
private:
    StateMachine sm_;
    PID pid_;
    SystemOutputs out_;
    double setpoint_;
};

} 