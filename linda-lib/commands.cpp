#include <array>

#ifdef _WIN32
#pragma once
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include "commands.hpp"

Command::Command(const std::string& name) {
    m.body["command"] = "gc." + name;

    // Defaults
    m.body["answer"] = json({});
    m.body["arguments"] = json({});
}

Message& Command::getMessage() {
    auto buf = m.body.dump();
    
    m.header.packtype = HEADER_PACKTYPE::COMMAND;
    m.header.packetsize = htonl(buf.size());
    return m;
}

std::ostream& operator<<(std::ostream& stream, const Command& c) {
    stream << c.m.body;
    return stream;
}

Temperature::Temperature(unsigned chips_bitmap) : Command("read_temperature") {
    json args;
    args["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

unsigned Temperature::getAnswer() {
    return m.body["answer"]["temperature"];
}
