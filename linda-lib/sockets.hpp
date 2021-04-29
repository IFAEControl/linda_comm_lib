#pragma once

#include <condition_variable>
#include <thread>

#include <nlohmann/json.hpp>

#include "commands.hpp"
#include "header.hpp"
#include "frame_buffer.hpp"

extern FrameBuffer fb;

class Networking {
public:
	void configure(std::string ip, unsigned port, unsigned aport);
	template<typename T> T sendCommand(T& c);
	void initReceiverThread();
	void joinThread();
private:
	void readerThread();

	std::string _ip{"8.8.8.8"};
	unsigned _port{32000}; 
	unsigned _async_port{32001};
	bool _thread_running{false};
	std::thread _reader{};
};