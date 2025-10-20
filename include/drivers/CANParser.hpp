// ...existing code...
#pragma once

// CANFrame is declared in CANSocket.hpp so do NOT redeclare it here.
#include "drivers/CANSocket.hpp"
#include "drivers/CANDefs.hpp" // contains CAN_ID_* constants used by parser

#include <cstdint>
#include <mutex>

namespace eae::drivers {

/**
 * @brief Container for the most recently decoded CAN values.
 *
 * Members hold the latest parsed values and are initialized to sensible
 * defaults so the system has known starting state before any CAN traffic.
 */
struct ParsedData {
    /// Latest measured temperature in degrees Celsius.
    double temperature_c{25.0};

    /// True when the fluid level sensor indicates sufficient level.
    bool level_ok{true};

    /// True when the pump is reported as running (feedback).
    bool pump_running{false};

    /// Optional override/setpoint temperature in degrees Celsius.
    double setpoint_c{25.0};
};

/**
 * @brief CANParser decodes incoming CAN frames into application-friendly data.
 *
 * Responsibilities:
 *  - Accept CANFrame objects (defined in CANSocket.hpp) via parseFrame().
 *  - Update an internal ParsedData instance when relevant frames are seen.
 *  - Provide thread-safe access to the latest parsed data via latest().
 *
 * Note: Do not redefine CANFrame here — that caused a redefinition error.
 */
class CANParser {
public:
    CANParser() = default;

    /**
     * @brief Parse a single CAN frame and update internal state if recognized.
     *
     * Returns true when the frame was recognized and used to update state,
     * false for irrelevant frames.
     */
    bool parseFrame(const CANFrame& frame);

    /**
     * @brief Return a copy of the most recently parsed data.
     *
     * Returns by value to avoid exposing internal state while allowing
     * safe concurrent access (protected by mutex).
     */
    ParsedData latest() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return data_;
    }

    // Convenience getters (copying the current value)
    double temperature() const { return latest().temperature_c; }
    bool ignition() const { return false; /* kept for compatibility; actual parsing may set this */ }
    bool level_ok() const { return latest().level_ok; }
    bool pump_feedback_ok() const { return latest().pump_running; }
    bool manual_reset() const { return false; /* placeholder; implement if needed */ }

private:
    mutable std::mutex mtx_;
    ParsedData data_;
};

} 