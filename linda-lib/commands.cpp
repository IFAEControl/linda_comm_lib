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

Temperature::Temperature() : Command("read_temperature") {
}

unsigned Temperature::getAnswer() {
    return m.body["answer"]["temperature"];
}

WriteChipConfig::WriteChipConfig(uint96_t& cfg) : Command("write_chip_config") {
    json args;
    args["chip_config"] = cfg.val();
    m.body["arguments"] = args;
}

uint96_t WriteChipConfig::getAnswer() {
    return m.body["answer"]["chip_config"];
}


WritePixelConfig::WritePixelConfig(uint17920_t& cfg) : Command("write_pixel_config") {
    json args;
    args["pixel_config"] = cfg.val();
    m.body["arguments"] = args;
}

uint17920_t WritePixelConfig::getAnswer() {
     return m.body["answer"]["pixel_config"];
}

PixelPulseWrite::PixelPulseWrite(uint1120_t& cfg) : Command("pixel_pulse_write") {
    json args;
    args["pixel_pulse"] = cfg.val();
    m.body["arguments"] = args;
}

uint1120_t PixelPulseWrite::getAnswer() {
    return m.body["answer"]["pixel_pulse"];
}

GCATReset::GCATReset() : Command("reset_gc_chip") {
}

PulsesGenerate::PulsesGenerate() : Command("generate_pulses") {
}

ResetLnaHpf::ResetLnaHpf(unsigned wait_time_us) : Command("reset_lna_hpf") {
    json args;
    args["wait_time_us"] = wait_time_us;
    m.body["arguments"] = args;
}

NeuronDrivingLna::NeuronDrivingLna(unsigned wait_time_us, unsigned reset_time_us,
                                    bool disable_reset) : Command("lna_neuron_driving") {
    json args;
    args["wait_time_us"] = wait_time_us;
    args["reset_time_us"] = reset_time_us;
    args["disable_reset"] = disable_reset;
    m.body["arguments"] = args;
}

NoNeuronDrivingLna::NoNeuronDrivingLna(unsigned wait_time_us, unsigned reset_time_us,
                                        bool disable_reset) : Command("lna_no_neuron_driving") {
    json args;
    args["wait_time_us"] = wait_time_us;
    args["reset_time_us"] = reset_time_us;
    args["disable_reset"] = disable_reset;
    m.body["arguments"] = args;
}

StatusPllOutReset::StatusPllOutReset() : Command("get_pll_out_reset_status") {
}

unsigned StatusPllOutReset::getAnswer() {
    return m.body["answer"]["status"];
}

ModePllBitstream::ModePllBitstream(unsigned mode) : Command("set_pll_bitstream_mode") {
    json args;
    args["mode"] = mode;
    m.body["arguments"] = args;
}

StartLna::StartLna(bool neuron_driving, unsigned period_ms, unsigned wait_time_us,
                        unsigned reset_time_us, bool disable_reset) : Command("start_lna_thread") {
    json args;
    args["neuron_driving"] = neuron_driving;
    args["period_ms"] = period_ms;
    args["wait_time_us"] = wait_time_us;
    args["reset_time_us"] = reset_time_us;
    args["disable_reset"] = disable_reset;
    m.body["arguments"] = args;
}

StopLna::StopLna() : Command("stop_lna_thread") {
}
