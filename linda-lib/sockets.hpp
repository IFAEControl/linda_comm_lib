#pragma once

#include <condition_variable>

#include <nlohmann/json.hpp>

#include "commands.hpp"
#include "header.hpp"

extern char* buffer;
extern unsigned bytes;
extern std::condition_variable cv;
extern std::mutex cv_m;
extern std::unique_lock<std::mutex> lk;

void init_thread();

std::unique_ptr<char> read_bytes(unsigned bytes);


Message send_command(Message& c);

template<typename T>
T send_command(T& c);

void set_dest_ip(const std::string& ip) noexcept;
void set_ports(unsigned p, unsigned ap) noexcept;
