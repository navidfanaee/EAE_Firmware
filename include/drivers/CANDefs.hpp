#pragma once
#include <cstdint>

namespace eae::drivers {
// defining Ids for cooling loop instruments
enum CAN_ID : uint32_t {
    CAN_ID_TEMPERATURE       = 0x100,
    CAN_ID_LEVEL             = 0x101,
    CAN_ID_PUMP_FEEDBACK     = 0x102,
    CAN_ID_SETPOINT_OVERRIDE = 0x200,
    CAN_ID_STATUS            = 0x300
};

struct TemperaturePayload {
    int16_t raw_centi; // centi-degrees big-endian
};

struct LevelPayload {
    uint8_t level; // 0 = LOW, 1 = OK
};

struct PumpFeedbackPayload {
    uint8_t running; // 0/1
};

struct SetpointPayload {
    uint8_t setpoint_degC; // integer 
};

} 