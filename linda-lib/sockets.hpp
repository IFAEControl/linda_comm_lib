#pragma once

#include <condition_variable>
#include <thread>

#include <nlohmann/json.hpp>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>

#include "commands.hpp"
#include "header.hpp"
#include "frame_buffer.hpp"

extern FrameBuffer fb;

class CmdSender {
public:
	CmdSender() =default;
	CmdSender(const std::string& ip, unsigned short p);
	template<typename T> T sendCommand(T& c);
private:
	Poco::Net::SocketAddress _sa;
	Poco::Net::StreamSocket _socket;
};

class Networking {
public:
	void configure(std::string ip, unsigned short port, unsigned short aport);
	template<typename T> T sendCommand(T& c);
	void initReceiverThread();
	void joinThread();
private:
	void readerThread();

	std::string _ip{"8.8.8.8"};
	unsigned short _port{32000}; 
	unsigned short _async_port{32001};
	bool _thread_running{false};
	std::thread _reader{};

	CmdSender _cmd_sender;
};