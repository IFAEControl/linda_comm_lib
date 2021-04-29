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

#define TEMPLATE_COMMAND(V) template V Networking::sendCommand<V>(V& c)

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

TEMPLATE_COMMAND(ReadTemperature);
TEMPLATE_COMMAND(SetHV);
TEMPLATE_COMMAND(SetTPDAC);
TEMPLATE_COMMAND(ChipRegisterWrite);
TEMPLATE_COMMAND(ChipRegisterRead);
TEMPLATE_COMMAND(PixelRegisterWrite);
TEMPLATE_COMMAND(PixelRegisterRead);
TEMPLATE_COMMAND(ReadEricaID);
TEMPLATE_COMMAND(FullArrayReadEricaID);
TEMPLATE_COMMAND(FullArrayChipRegisterRead);
TEMPLATE_COMMAND(FullArrayPixelRegisterRead);
TEMPLATE_COMMAND(ACQuisition);
TEMPLATE_COMMAND(ACQuisitionCont);
TEMPLATE_COMMAND(ACQuisitionStop);
TEMPLATE_COMMAND(CameraReset);
TEMPLATE_COMMAND(ControllerReset);
TEMPLATE_COMMAND(LoadFloodNormFactors);