#include <gtest/gtest.h>
#include "app/CoolingController.hpp"
#include "drivers/CANParser.hpp"

using namespace eae::app;
using namespace eae::drivers;

class FakeCAN : public CANParser {
public:
    void set(double temp, bool ign, bool lvl, bool pump, bool reset=false) {
        temp_ = temp; ign_ = ign; lvl_ = lvl; pump_ = pump; reset_ = reset;
    }
    double temperature() const { return temp_; }
    bool ignition() const { return ign_; }
    bool level_ok() const { return lvl_; }
    bool pump_feedback_ok() const { return pump_; }
    bool manual_reset() const { return reset_; }
private:
    double temp_{};
    bool ign_{}, lvl_{}, pump_{}, reset_{};
};

TEST(CoolingController, NormalOperation) {
    FakeCAN can;
    CoolingController ctrl;

    can.set(60.0, true, true, true);
    ctrl.update(can, 0);
    EXPECT_TRUE(ctrl.outputs().pump_cmd);

    can.set(90.0, true, true, true);
    ctrl.update(can, 100);
    EXPECT_GE(ctrl.outputs().fan_pwm, 0);
}
