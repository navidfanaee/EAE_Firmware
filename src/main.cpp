#include "drivers/CANParser.hpp"
#include "app/CoolingController.hpp"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>

using namespace eae::drivers;
using namespace eae::app;
using namespace std::chrono;

static bool keep_running = true;
void sigint_handler(int) { keep_running = false; }

int main() {
    signal(SIGINT, sigint_handler);

    // setup CAN socket
    int sock = socket(PF_CAN, SOCK_RAW | SOCK_NONBLOCK, CAN_RAW);
    if (sock < 0) { perror("socket"); return 1; }

    struct ifreq ifr {};
    std::strncpy(ifr.ifr_name, "vcan0", IFNAMSIZ);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) { perror("ioctl"); return 1; }

    struct sockaddr_can addr {};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) { perror("bind"); return 1; }

    std::cout << "[INFO] Connected to vcan0\n";

    CANParser parser;
    CoolingController controller;

    struct can_frame frame;
    const double dt_sec = 0.1; // 100 ms control loop
    auto next = steady_clock::now();

        SystemState last_state = SystemState::OFF;
    while (keep_running) {
        int n = read(sock, &frame, sizeof(frame));
        if (n > 0) {
            CANFrame f;
            f.id = frame.can_id & CAN_EFF_MASK;
            f.data.assign(frame.data, frame.data + frame.can_dlc);
            f.ts_ms = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
            parser.parseFrame(f);
        }

        uint64_t now_ms = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
        controller.update(parser, now_ms);
        auto out = controller.outputs();

        // print only when something meaningful changes or periodically
        static uint64_t last_print = 0;
        if (out.state != last_state || now_ms - last_print > 1000) {
            std::cout << "[CTRL] "
                      << "State=" << controller.state_name()
                      << "  T=" << parser.temperature()
                      << "°C  Fan=" << out.fan_pwm
                      << "%  Pump=" << (out.pump_cmd ? "ON" : "OFF")
                      << "  Derate=" << (out.inverter_derate ? "Y" : "N")
                      << "\n";
            last_state = out.state;
            last_print = now_ms;
        }

        next += milliseconds(static_cast<int>(dt_sec * 1000));
        std::this_thread::sleep_until(next);
    }


    close(sock);
    std::cout << "Shutting down...\n";
    return 0;
}
