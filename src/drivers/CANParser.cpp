#include "drivers/CANParser.hpp"
#include "drivers/CANDefs.hpp"
#include <mutex>

namespace eae::drivers {

// Decode incoming CAN frames and update internal state.
bool CANParser::parseFrame(const CANFrame& f)
{
    std::lock_guard<std::mutex> lk(mtx_);

    switch (f.id) {
    case CAN_ID_TEMPERATURE:
        // 2-byte signed int16 in centi-degrees (big-endian)
        data_.temperature_c = static_cast<double>((f.data[0] << 8) | f.data[1]) / 100.0;
        return true;
        // parse level status from CAN frame
    case CAN_ID_LEVEL:
        data_.level_ok = (f.data[0] != 0);
        return true;
        // parse pump feedback status from CAN frame
    case CAN_ID_PUMP_FEEDBACK:
        data_.pump_running = (f.data[0] != 0);
        return true;
        // parse setpoint override from CAN frame
    case CAN_ID_SETPOINT_OVERRIDE:
        data_.setpoint_c = static_cast<double>(f.data[0]);
        return true;

    default:
        return false;
    }
}

} // namespace eae::drivers
