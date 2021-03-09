#include <array>

#ifdef _WIN32
#pragma once
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include "commands.hpp"

Command::Command(const std::string& name) {
    m.body["command"] = "ctrl." + name;

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

HVSet::HVSet(unsigned counts) : Command("set_hv") {
    json args;
    args["counts"] = counts;
    m.body["arguments"] = args;
}

TPDACSet::TPDACSet(unsigned counts) : Command("set_tpdac") {
    json args;
    args["counts"] = counts;
    m.body["arguments"] = args;
}

WriteChipRegister::WriteChipRegister(uint156_t& val, unsigned chips_bitmap) : Command("chip_reg_write") {
    json args;
    args["chip_reg"] = val.val();
    m.body["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

ReadChipRegister::ReadChipRegister(unsigned chips_bitmap) : Command("chip_reg_read") {
    json args;
    args["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

uint156_t ReadChipRegister::getAnswer() {
    return m.body["answer"]["chip_reg"];
}

ReadFullArrayChipRegister::ReadFullArrayChipRegister(unsigned chips_bitmap) : Command("full_chip_reg_read") {
    json args;
    args["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

LongInt<150> ReadFullArrayChipRegister::getAnswer() {
    return m.body["answer"]["chip_reg"];
}

WritePixelRegister::WritePixelRegister(uint15360_t& val, unsigned chips_bitmap) : Command("pixel_reg_write") {
    json args;
    args["pixel_reg"] = val.val();
    m.body["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

ReadPixelRegister::ReadPixelRegister(unsigned chips_bitmap) : Command("pixel_reg_read") {
    json args;
    args["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

uint15360_t ReadPixelRegister::getAnswer() {
    return m.body["answer"]["pixel_reg"];
}

ReadFullArrayPixelRegister::ReadFullArrayPixelRegister(unsigned chips_bitmap) : Command("full_pixel_reg_read") {
    json args;
    args["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

LongInt<14400> ReadFullArrayPixelRegister::getAnswer() {
    return m.body["answer"]["pixel_reg"];
}

ChipIDRead::ChipIDRead(unsigned chips_bitmap) : Command("read_chip_id") {
    json args;
    args["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

unsigned ChipIDRead::getAnswer() {
    return m.body["answer"]["chip_id"];
}

FullChipIDRead::FullChipIDRead(unsigned chips_bitmap) : Command("read_full_chip_id") {
    json args;
    args["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

LongInt<30> FullChipIDRead::getAnswer() {
    return m.body["answer"]["chip_id"];
}

NonTdiAcq::NonTdiAcq(const unsigned belt_dir, unsigned chips_bitmap) : Command("acq_non_tdi") {
    json args;
    args["belt_dir"] = belt_dir;
    args["chips_bitmap"] = chips_bitmap;
    m.body["arguments"] = args;
}

LongInt<480> NonTdiAcq::getAnswer() {
    return m.body["answer"]["frame"];
}

