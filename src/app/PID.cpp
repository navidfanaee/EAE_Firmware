#include "app/PID.hpp"
#include <algorithm>
#include <cmath>

using namespace eae::app;

PID::PID(double kp, double ki, double kd, double out_min, double out_max)
: kp_(kp), ki_(ki), kd_(kd),
  integrator_(0.0), prev_measurement_(0.0),
  out_min_(out_min), out_max_(out_max) {}

void PID::reset() {
    integrator_ = 0.0;
    prev_measurement_ = 0.0;
}

void PID::setLimits(double minv, double maxv) {
    out_min_ = minv;
    out_max_ = maxv;
}

void PID::setTunings(double kp, double ki, double kd) {
    kp_ = kp; ki_ = ki; kd_ = kd;
}

double PID::clamp(double v, double lo, double hi) const {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

double PID::update(double setpoint, double measurement, double dt) {
    if (dt <= 0.0) return 0.0;
    if (prev_measurement_ == 0.0 && measurement != 0.0) {
        prev_measurement_ = measurement;
    }
    double error = setpoint - measurement;

    // P term
    double P = kp_ * error;

    // I term
    integrator_ += error * dt;
    double I = ki_ * integrator_;

    // Derivative 
    double derivative = (measurement - prev_measurement_) / dt;
    double D = -kd_ * derivative;


    double raw_out = P + I + D;

    // Anti-windup: if output is outside limits, reduce integrator slightly
    // We apply a simple back-off: if raw_out > max and error > 0 => reduce integrator
    if (raw_out > out_max_ && error > 0.0) {
        // back off integrator proportionally (small factor)
        integrator_ -= error * dt * 0.5;
        I = ki_ * integrator_;
    } else if (raw_out < out_min_ && error < 0.0) {
        integrator_ -= error * dt * 0.5;
        I = ki_ * integrator_;
    }

    double out = P + I + D;
    out = clamp(out, out_min_, out_max_);

    prev_measurement_ = measurement;
    return out;
}