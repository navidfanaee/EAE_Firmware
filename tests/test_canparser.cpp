#include "drivers/CANParser.hpp"
#include "drivers/CANDefs.hpp"
#include <gtest/gtest.h>
using namespace eae::drivers;

// test for parsing temperature CAN frame
TEST(CANParser, ParsesTemperature) {
    CANParser parser;
    CANFrame f{CAN_ID_TEMPERATURE, {0x0F, 0xA0}}; // 4000 => 40.00 °C
    ASSERT_TRUE(parser.parseFrame(f));
    EXPECT_NEAR(parser.latest().temperature_c, 40.0, 0.01);
}
// test for parsing level CAN frame
TEST(CANParser, ParsesLevel) {
    CANParser parser;
    CANFrame f{CAN_ID_LEVEL, {0x01}}; // level OK
    ASSERT_TRUE(parser.parseFrame(f));
    EXPECT_TRUE(parser.latest().level_ok);

    f.data[0] = 0x00; // level NOT OK
    ASSERT_TRUE(parser.parseFrame(f));
    EXPECT_FALSE(parser.latest().level_ok);
}
// test for parsing pump feedback CAN frame
TEST(CANParser, ParsesPumpFeedback) {
    CANParser parser;
    CANFrame f{CAN_ID_PUMP_FEEDBACK, {0x01}}; // pump running
    ASSERT_TRUE(parser.parseFrame(f));
    EXPECT_TRUE(parser.latest().pump_running);

    f.data[0] = 0x00; // pump not running
    ASSERT_TRUE(parser.parseFrame(f));
    EXPECT_FALSE(parser.latest().pump_running);
}
