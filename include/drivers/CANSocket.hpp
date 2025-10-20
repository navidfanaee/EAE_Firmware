#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace eae::drivers {

struct CANFrame {
    uint32_t id{};
    std::vector<uint8_t> data;
    uint64_t ts_ms{};
};

class CANSocket {
public:
    explicit CANSocket(const std::string& ifname = "vcan0") : iface_(ifname) {}
    bool open_socket() { return true; } // stub
    bool recv_frame(CANFrame&) { return false; } // stub: no data
    bool send_frame(const CANFrame&) { return true; } // stub
private:
    std::string iface_;
};

} // namespace eae::drivers
