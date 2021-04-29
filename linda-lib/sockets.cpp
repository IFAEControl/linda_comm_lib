#include <iostream>
#include <thread>
#include <sstream>
#include <memory>
#include <cstring>

#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/StreamCopier.h>

#include "sockets.hpp"

using Poco::Net::IPAddress;
using namespace CMD;

static std::string ip = "8.8.8.8";
unsigned port = 32000;
unsigned async_port = 32001;

FrameBuffer fb;

static bool thread_running = false;
std::thread reader{};

void reader_thread();

void init_thread() {
    if(!thread_running) {
        thread_running = true;
        reader = std::thread(reader_thread);
    }
}

void join_thread() {
    if(thread_running)
        reader.join();
}


void reader_thread() {
    Poco::Net::SocketAddress sa(ip, async_port);
    Poco::Net::StreamSocket dgs(sa);
    dgs.setBlocking(true);

    for (;;) {
        unsigned bytes = 0;    
        // first read how many bytes to read
        dgs.receiveBytes(&bytes, sizeof(bytes), MSG_WAITALL);

        Frame f(bytes);
        int n = dgs.receiveBytes(f.get(), bytes, MSG_WAITALL);
        fb.addFrame(std::move(f));
    } 
}

void set_dest_ip(const std::string& str) noexcept {
    ip = str;
}

void set_ports(unsigned p, unsigned ap) noexcept {
    port = p;
    async_port = ap;
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

template ReadTemperature send_command<ReadTemperature>(ReadTemperature& c);
template SetHV send_command<SetHV>(SetHV& c);
template SetTPDAC send_command<SetTPDAC>(SetTPDAC& c);
template ChipRegisterWrite send_command<ChipRegisterWrite>(ChipRegisterWrite& c);
template ChipRegisterRead send_command<ChipRegisterRead>(ChipRegisterRead& c);
template PixelRegisterWrite send_command<PixelRegisterWrite>(PixelRegisterWrite& c);
template PixelRegisterRead send_command<PixelRegisterRead>(PixelRegisterRead& c);
template ReadEricaID send_command<ReadEricaID>(ReadEricaID& c);
template FullArrayReadEricaID send_command<FullArrayReadEricaID>(FullArrayReadEricaID& c);
template FullArrayChipRegisterRead send_command<FullArrayChipRegisterRead>(FullArrayChipRegisterRead& c);
template FullArrayPixelRegisterRead send_command<FullArrayPixelRegisterRead>(FullArrayPixelRegisterRead& c);
template ACQuisition send_command<ACQuisition>(ACQuisition& c);
template ACQuisitionCont send_command<ACQuisitionCont>(ACQuisitionCont& c);
template ACQuisitionStop send_command<ACQuisitionStop>(ACQuisitionStop& c);
template CameraReset send_command<CameraReset>(CameraReset& c);
template ControllerReset send_command<ControllerReset>(ControllerReset& c); 
template LoadFloodNormFactors send_command<LoadFloodNormFactors>(LoadFloodNormFactors& c);