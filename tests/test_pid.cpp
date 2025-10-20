#include <gtest/gtest.h>
#include "app/PID.hpp"
#include <cmath>

using namespace eae::app;

TEST(PID, ProportionalOnly) {
    PID pid(2.0, 0.0, 0.0, 0.0, 100.0);
    pid.reset();
    double out = pid.update(100.0, 90.0, 0.1); // error 10 -> P = 20
    EXPECT_NEAR(out, 20.0, 1e-6);
}

TEST(PID, IntegratorAccumulation) {
    PID pid(0.0, 1.0, 0.0, -1e6, 1e6); // only I = 1.0
    pid.reset();
    double out = 0.0;
    // apply constant error of 2.0 for 5 steps with dt=0.1 -> integrator ~ 2.0*0.5 = 1.0
    for (int i=0;i<5;i++) {
        out = pid.update(10.0, 8.0, 0.1); // error = 2
    }
    // Expect approx I = ki * integral = 1.0 * (2 * 0.5) = 1.0 (since 5*0.1*2 = 1.0)
    EXPECT_NEAR(out, 1.0, 1e-3);
}

TEST(PID, DerivativeReducesOnRisingMeasurement) {
    PID pid(1.0, 0.0, 1.0, -1e6, 1e6);
    pid.reset();
    double out1 = pid.update(50.0, 40.0, 0.1); // baseline
    double out2 = pid.update(50.0, 41.0, 0.1); // measurement rose by 1 -> derivative positive -> D negative
    // expect out2 < out1 (derivative reduces output when measurement increases)
    EXPECT_LT(out2, out1);
}

TEST(PID, SaturationAndAntiWindup) {
    PID pid(1.0, 2.0, 0.0, 0.0, 10.0); // limited output 0..10, strong I
    pid.reset();
    double out = 0.0;
    for (int i=0;i<50;i++) {
        out = pid.update(100.0, 0.0, 0.1);
    }
    // Output must be clamped to 10
    EXPECT_NEAR(out, 10.0, 1e-3);

    // Now reverse the error so integrator should come back down and output reduce from saturation.
    for (int i=0;i<50;i++) {
        out = pid.update(0.0, 100.0, 0.1);
    }
    EXPECT_LE(out, 10.0);
}
