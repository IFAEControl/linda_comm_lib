#include <iostream>
#include <sstream>
#include <cstring>

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/StreamCopier.h>

#include "sockets.hpp"

using Poco::Net::IPAddress;

static std::string ip = "8.8.8.8";
constexpr unsigned port = 32000;

void set_dest_ip(const std::string& str) noexcept {
    ip = str;
}


Message send_command(Message& c) {
    Poco::Net::SocketAddress sa(ip, port);
    Poco::Net::StreamSocket socket(sa);
    socket.sendBytes(&c.header, HEADER_BYTE_SIZE);
    Poco::Net::SocketStream str(socket);
    str << c.body;
    str.flush();
    std::stringstream ss;
    Poco::StreamCopier::copyStream(str, ss);
    std::memcpy(&c.header, ss.str().c_str(), HEADER_BYTE_SIZE);
    c.body = json::parse(ss.seekg(HEADER_BYTE_SIZE));
    return c;
}

template<typename T>
T send_command(T& c) {
    auto& m = c.getMessage();

    Poco::Net::SocketAddress sa(ip, port);
    Poco::Net::StreamSocket socket(sa);
    socket.sendBytes(&m.header, HEADER_BYTE_SIZE);
    Poco::Net::SocketStream str(socket);
    str << m.body;
    str.flush();
    std::stringstream ss;
    Poco::StreamCopier::copyStream(str, ss);
    std::memcpy(&m.header, ss.str().c_str(), HEADER_BYTE_SIZE);
    m.body = json::parse(ss.seekg(HEADER_BYTE_SIZE));
    return c;
}

template Temperature send_command<Temperature>(Temperature& c);
template WriteChipConfig send_command<WriteChipConfig>(WriteChipConfig& c);
template WritePixelConfig send_command<WritePixelConfig>(WritePixelConfig& c);
template PixelPulseWrite send_command<PixelPulseWrite>(PixelPulseWrite& c);
template PulsesGenerate send_command<PulsesGenerate>(PulsesGenerate& c);
template GCATReset send_command<GCATReset>(GCATReset& c);
template NeuronDrivingLna send_command<NeuronDrivingLna>(NeuronDrivingLna& c);
template NoNeuronDrivingLna send_command<NoNeuronDrivingLna>(NoNeuronDrivingLna& c);
template StatusPllOutReset send_command<StatusPllOutReset>(StatusPllOutReset& c);
template ModePllBitstream send_command<ModePllBitstream>(ModePllBitstream& c);
template ResetLnaHpf send_command<ResetLnaHpf>(ResetLnaHpf& c);
template StartLna send_command<StartLna>(StartLna& c);
template StopLna send_command<StopLna>(StopLna& c);

