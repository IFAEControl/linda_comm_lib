#include <cstring>
#include <iostream>

#include "frame_buffer.hpp"

// XXX: make it thread-safe and avoid burning cycles

char* FrameBuffer::addFrame(unsigned size) {
	if(_curr_write_frame == _curr_read_frame) {
		// we could reuse the same memory allocation to save time
		auto frame = _buf[_curr_read_frame];
		delete[] frame.mem;
		_curr_read_frame = ++_curr_read_frame % CACHE_SIZE;
	}

	auto addr = new char[size];
	_buf[_curr_write_frame] = Frame{addr, size};

	_curr_write_frame = ++_curr_write_frame % CACHE_SIZE;
	return addr;
}

void FrameBuffer::moveLastFrame(unsigned* data) {
	//std::cout << "Write " << _curr_write_frame << std::endl;
	//std::cout << "Read " << _curr_read_frame << std::endl;

	// wait for a frame	
	while(_buf[_curr_read_frame].mem == nullptr);

	auto frame = _buf[_curr_read_frame];
	std::memcpy(data, frame.mem, frame.bytes);
	delete[] frame.mem;
	_buf[_curr_read_frame].mem = nullptr;
	_curr_read_frame = ++_curr_read_frame % CACHE_SIZE;
}
