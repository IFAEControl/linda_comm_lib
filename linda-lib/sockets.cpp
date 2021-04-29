#include <iostream>
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

FrameBuffer fb;

void Networking::configure(std::string ip, unsigned port, unsigned aport) {
    _ip = ip;
    _port = port;
    _async_port = aport;
}


void Networking::initReceiverThread() {
    if(!_thread_running) {
        _thread_running = true;
        _reader = std::thread(&Networking::readerThread, this);
    }
}

void Networking::joinThread() {
    if(_thread_running)
        _reader.join();
}

void Networking::readerThread() {
    Poco::Net::SocketAddress sa(_ip, _async_port);
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

template<typename T>
T Networking::sendCommand(T& c) {
    auto& m = c.getMessage();

    Poco::Net::SocketAddress sa(_ip, _port);
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

template ReadTemperature Networking::sendCommand<ReadTemperature>(ReadTemperature& c);
template SetHV Networking::sendCommand<SetHV>(SetHV& c);
template SetTPDAC Networking::sendCommand<SetTPDAC>(SetTPDAC& c);
template ChipRegisterWrite Networking::sendCommand<ChipRegisterWrite>(ChipRegisterWrite& c);
template ChipRegisterRead Networking::sendCommand<ChipRegisterRead>(ChipRegisterRead& c);
template PixelRegisterWrite Networking::sendCommand<PixelRegisterWrite>(PixelRegisterWrite& c);
template PixelRegisterRead Networking::sendCommand<PixelRegisterRead>(PixelRegisterRead& c);
template ReadEricaID Networking::sendCommand<ReadEricaID>(ReadEricaID& c);
template FullArrayReadEricaID Networking::sendCommand<FullArrayReadEricaID>(FullArrayReadEricaID& c);
template FullArrayChipRegisterRead Networking::sendCommand<FullArrayChipRegisterRead>(FullArrayChipRegisterRead& c);
template FullArrayPixelRegisterRead Networking::sendCommand<FullArrayPixelRegisterRead>(FullArrayPixelRegisterRead& c);
template ACQuisition Networking::sendCommand<ACQuisition>(ACQuisition& c);
template ACQuisitionCont Networking::sendCommand<ACQuisitionCont>(ACQuisitionCont& c);
template ACQuisitionStop Networking::sendCommand<ACQuisitionStop>(ACQuisitionStop& c);
template CameraReset Networking::sendCommand<CameraReset>(CameraReset& c);
template ControllerReset Networking::sendCommand<ControllerReset>(ControllerReset& c); 
template LoadFloodNormFactors Networking::sendCommand<LoadFloodNormFactors>(LoadFloodNormFactors& c);