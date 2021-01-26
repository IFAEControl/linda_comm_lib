#include <iostream>
#include <array>
#include <algorithm>
#include <utility>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "linda-lib.hpp"
#include "commands.hpp"
#include "sockets.hpp"

template <typename T>
std::pair<int, T> sendCmd(T& cmd) try {
    spdlog::set_level(spdlog::level::debug);
    
    auto resp = send_command(cmd);
    spdlog::debug(resp);
    return {0, resp};
} catch(std::exception& e) {
    spdlog::critical(e.what());
    return {-1, cmd};
} catch(...) {
    spdlog::critical("Unknown error");
    return {-2, cmd};
}

void InitCommunication(const char* str, unsigned sync_port, unsigned async_port) {
    set_dest_ip(str);
    set_ports(sync_port, async_port);
}

int ReadTemperature(unsigned* temp, unsigned chips_bitmap) {
    if(!temp)
        return -1;

    Temperature cmd(chips_bitmap);
    auto resp = sendCmd(cmd);
    if(resp.first < 0) return resp.first;

    *temp = resp.second.getAnswer();
    return resp.first;
}
