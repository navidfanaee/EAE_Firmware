#pragma once

namespace eae::app {
// PID controller class for temperature regulation
class PID {
public:
    PID(double kp = 1.2, double ki = 0.1, double kd = 0.05, double out_min = 0.0, double out_max = 100.0);

    void reset();
    double update(double setpoint, double measurement, double dt);
    void setLimits(double minv, double maxv);
    void setTunings(double kp, double ki, double kd);

private:
    double kp_;
    double ki_;
    double kd_;

    double integrator_;
    double prev_measurement_;
    double out_min_;
    double out_max_;

    // small helper to clamp
    double clamp(double v, double lo, double hi) const;
};

} 