#include <cstring>
#include <iostream>
#include <mutex>

#include "frame_buffer.hpp"

std::mutex _mutex;

// XXX: make it thread-safe and avoid burning cycles

char* FrameBuffer::addFrame(unsigned size) {
	auto addr = new char[size];
	_buf[_curr_write_frame] = Frame{addr, size};

	_curr_write_frame = ++_curr_write_frame % CACHE_SIZE;

	if(_curr_write_frame == _curr_read_frame) {
		// we could reuse the same memory allocation to save time
		auto frame = _buf[_curr_read_frame];
		delete[] frame.mem;
		_curr_read_frame = ++_curr_read_frame % CACHE_SIZE;
	}

	return addr;
}

void FrameBuffer::moveLastFrame(unsigned* data) {

	// wait for a frame	
	while(_buf[_curr_read_frame].mem == nullptr) {
	}

	_mutex.lock();
	
	auto frame = _buf[_curr_read_frame];
	std::memcpy(data, frame.mem, frame.bytes);
	delete[] frame.mem;
	_buf[_curr_read_frame].mem = nullptr;
	_curr_read_frame = ++_curr_read_frame % CACHE_SIZE;

	_mutex.unlock();
}
