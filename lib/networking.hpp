#pragma once

#include <condition_variable>
#include <thread>

#include <nlohmann/json.hpp>
#include <Poco/Net/DatagramSocket.h>
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
	template<typename T> T sendCommand(T&& c);
private:
	Poco::Net::SocketAddress _sa;
	Poco::Net::StreamSocket _socket;
};

class DataReceiver {
public:
	DataReceiver() =default;
	DataReceiver(const std::string& ip, unsigned short port);
	void initThread();
	void joinThread();
private:
	void readerThread();
	void connect();

	bool _thread_running{false};
	std::thread _reader{};

	Poco::Net::SocketAddress _sa;
    Poco::Net::DatagramSocket _dgs;

};

class Networking {
public:
	void configure(std::string ip, unsigned short port, unsigned short aport);
	template<typename T> T sendCommand(T&& c);
	void initReceiverThread();
	void joinThread();
private:
	std::string _ip{"8.8.8.8"};
	CmdSender _cmd_sender;
	DataReceiver _data_receiver;
};