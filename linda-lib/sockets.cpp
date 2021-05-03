#include <iostream>
#include <sstream>
#include <memory>
#include <cstring>

#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/StreamCopier.h>

#include "sockets.hpp"
#include "log.hpp"

#define TEMPLATE_COMMAND(V) template V Networking::sendCommand<V>(V&& c)

using Poco::Net::IPAddress;
using namespace CMD;

FrameBuffer fb;

CmdSender::CmdSender(const std::string& ip, unsigned short p) :
    _sa{ip, p}
{}

template<typename T>
T CmdSender::sendCommand(T&& c) {
    auto& m = c.getMessage();
    _socket.connect(_sa);
    _socket.sendBytes(&m.header, HEADER_BYTE_SIZE);
    Poco::Net::SocketStream str(_socket);
    str << m.body;
    str.flush();
    std::stringstream ss;
    Poco::StreamCopier::copyStream(str, ss);
    std::memcpy(&m.header, ss.str().c_str(), HEADER_BYTE_SIZE);
    if(m.header.packtype == HEADER_PACKTYPE::ERROR)
        throw std::runtime_error("Command error");
    m.body = json::parse(ss.seekg(HEADER_BYTE_SIZE));
    _socket.close();
    return std::move(c);
}

DataReceiver::DataReceiver(const std::string& ip, unsigned short p) :
    _sa{ip, p}
{}

void DataReceiver::initThread() {
    if(!_thread_running) {
        _thread_running = true;
        _reader = std::thread(&DataReceiver::readerThread, this);
    }
}


void DataReceiver::readerThread() {
    Poco::Net::StreamSocket dgs(_sa);
    dgs.setBlocking(true);

    while(_thread_running) {
        unsigned bytes = 0;  
        HEADER_PACKTYPE type;

        dgs.receiveBytes(&type, sizeof(type), MSG_WAITALL);
        if(type == HEADER_PACKTYPE::ERROR) {
            logger->error("Error on asyncs");
            dgs.close();
            dgs.connect(_sa);
            continue;
        }

        // first read how many bytes to read
        dgs.receiveBytes(&bytes, sizeof(bytes), MSG_WAITALL);

        Frame f(bytes);
        int n = dgs.receiveBytes(f.get(), bytes, MSG_WAITALL);
        fb.addFrame(std::move(f));
    } 
}

void DataReceiver::joinThread() {
    if(_thread_running)
        _reader.join();
}

void Networking::configure(std::string ip, unsigned short port, unsigned short aport) {
    _ip = ip;
    _cmd_sender = CmdSender(_ip, port);
    _data_receiver = DataReceiver(_ip, aport);
}


void Networking::initReceiverThread() {
    _data_receiver.initThread();
}

void Networking::joinThread() {
    _data_receiver.joinThread();
}

template<typename T>
T Networking::sendCommand(T&& c) {
    return std::move(_cmd_sender.sendCommand(std::move(c)));
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