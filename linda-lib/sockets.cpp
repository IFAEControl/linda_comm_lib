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
unsigned port = 32000;
unsigned async_port;

void set_dest_ip(const std::string& str) noexcept {
    ip = str;
}

void set_ports(unsigned p, unsigned ap) noexcept {
    port = p;
    async_port = ap;
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
template HVSet send_command<HVSet>(HVSet& c);
template TPDACSet send_command<TPDACSet>(TPDACSet& c);
template WriteChipRegister send_command<WriteChipRegister>(WriteChipRegister& c);
template ReadChipRegister send_command<ReadChipRegister>(ReadChipRegister& c);
template WritePixelRegister send_command<WritePixelRegister>(WritePixelRegister& c);
template ReadPixelRegister send_command<ReadPixelRegister>(ReadPixelRegister& c);
template ChipIDRead send_command<ChipIDRead>(ChipIDRead& c);
template FullChipIDRead send_command<FullChipIDRead>(FullChipIDRead& c);
template ReadFullArrayChipRegister send_command<ReadFullArrayChipRegister>(ReadFullArrayChipRegister& c);
template ReadFullArrayPixelRegister send_command<ReadFullArrayPixelRegister>(ReadFullArrayPixelRegister& c);
template NonTdiAcq send_command<NonTdiAcq>(NonTdiAcq& c);
