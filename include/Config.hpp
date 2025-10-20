#pragma once
namespace eae::config {
    inline constexpr double CONTROL_DT_SEC = 0.2; // 200 ms
    inline constexpr double DEFAULT_SETPOINT_C = 65.0;
    inline constexpr double HYSTERESIS_C = 2.0;
    inline constexpr double DERATE_TEMP_C = 75.0;
    inline constexpr double CRITICAL_TEMP_C = 95.0;
    inline constexpr int SENSOR_TIMEOUT_MS = 400;
    inline constexpr int PUMP_FEEDBACK_TIMEOUT_MS = 1000;
    inline constexpr int WATCHDOG_TIMEOUT_MS = 500;
    inline constexpr int PUMP_PRIME_MS = 3000;
    inline constexpr int BOOST_DURATION_MS = 30000;
}
