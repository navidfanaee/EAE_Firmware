#include "app/CoolingController.hpp"
#include "drivers/CANParser.hpp"
#include <gtest/gtest.h>

using namespace eae::app;
using namespace eae::drivers;

TEST(CoolingController, NormalOperation)
{
    CoolingController ctrl;
    CANParser parser;

    // Fabricate CAN values consistent with nominal running conditions
    CANFrame temp{0x100, {0x1A, 0x90}}; // 0x1A90=6800 => 68.00°C
    CANFrame lvl {0x101, {1}};
    CANFrame pump{0x102, {1}};
    CANFrame ign {0x103, {1}};
    CANFrame reset{0x104, {0}};

    parser.parseFrame(temp);
    parser.parseFrame(lvl);
    parser.parseFrame(pump);
    parser.parseFrame(ign);
    parser.parseFrame(reset);

    ctrl.update(parser, /*now_ms=*/100);

    auto out = ctrl.outputs();
    // Expect system to leave OFF and command the pump ON
    EXPECT_TRUE(out.pump_cmd);
    EXPECT_GE(out.fan_pwm, 0);
}
