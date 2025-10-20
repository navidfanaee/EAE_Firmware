#pragma once
#include <cstdint>
#include <mutex>
#include <vector>

namespace eae::drivers {

/// defining can data frame structure contatining id and data payload
struct CANFrame {
    uint32_t id;
    std::vector<uint8_t> data;
};
// defining sensor data structure to hold parsed values 
struct SensorData {
    double temperature_c{25.0};
    bool   level_ok{true};
    bool   pump_running{false};
    double setpoint_c{65.0};
};
// CAN message parser class and using mutex for thread safety
class CANParser {
public:
    bool parseFrame(const CANFrame& f);
    SensorData latest() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return data_;
    }
private:
    mutable std::mutex mtx_;
    SensorData data_;
};

} 
