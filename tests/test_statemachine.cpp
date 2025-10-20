#include <gtest/gtest.h>
#include "app/StateMachine.hpp"

using namespace eae::app;

TEST(StateMachine, OffToStartup) {
    StateMachine sm;
    SystemInputs in{25.0, true, true, false, false, false, 0};
    auto out = sm.step(in);
    EXPECT_EQ(out.state, SystemState::STARTUP_PRIME);
}

TEST(StateMachine, StartupToIdleWhenPumpOk) {
    StateMachine sm;
    sm.step({25.0, true, true, false, false, false, 0}); // OFF->STARTUP
    auto out = sm.step({25.0, true, true, true, false, false, 100});
    EXPECT_EQ(out.state, SystemState::IDLE);
}

TEST(StateMachine, IdleToCoolingOnHighTemp) {
    StateMachine sm;
    sm.step({25.0, true, true, true, false, false, 0});
    sm.step({25.0, true, true, true, false, false, 10}); // IDLE
    auto out = sm.step({70.0, true, true, true, false, false, 20});
    EXPECT_EQ(out.state, SystemState::ACTIVE_COOLING);
}

TEST(StateMachine, CoolingToBoost) {
    StateMachine sm;
    sm.step({25.0, true, true, true, false, false, 0});
    sm.step({70.0, true, true, true, false, false, 10}); // IDLE->ACTIVE
    sm.step({85.0, true, true, true, false, false, 20});
    auto out = sm.step({95.0, true, true, true, false, false, 30});
    EXPECT_EQ(out.state, SystemState::BOOST_COOLING);
}

TEST(StateMachine, FaultReset) {
    StateMachine sm;
    // First: OFF -> STARTUP_PRIME
    sm.step({25.0, true, true, false, false, false, 0});
    // Now: level low should cause FAULT
    sm.step({25.0, true, false, false, false, false, 10});
    // Now reset with level and pump OK
    auto out = sm.step({25.0, true, true, true, false, true, 50});
    EXPECT_EQ(out.state, SystemState::STARTUP_PRIME);
}

