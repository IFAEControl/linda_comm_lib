#pragma once

#include <condition_variable>

#include <nlohmann/json.hpp>

#include "commands.hpp"
#include "header.hpp"
#include "frame_buffer.hpp"

extern FrameBuffer fb;

void init_thread();

template<typename T>
T send_command(T& c);

void set_dest_ip(const std::string& ip) noexcept;
void set_ports(unsigned p, unsigned ap) noexcept;
