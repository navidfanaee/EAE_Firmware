#pragma once
// defining CAN message IDs used in the system
namespace eae::drivers {
    constexpr unsigned CAN_ID_TEMPERATURE       = 0x100;
    constexpr unsigned CAN_ID_LEVEL             = 0x101;
    constexpr unsigned CAN_ID_PUMP_FEEDBACK     = 0x102;
    constexpr unsigned CAN_ID_SETPOINT_OVERRIDE = 0x200;
}
