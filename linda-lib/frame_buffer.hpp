#pragma once

#include <atomic>
#include <cstddef>

constexpr unsigned CACHE_SIZE = 33101;

extern std::mutex _mutex;


struct Frame {
	char* mem;
	unsigned bytes;
};

class FrameBuffer {
public:
	char* addFrame(unsigned size);
	void moveLastFrame(unsigned* data);
private:
	Frame _buf[CACHE_SIZE]{};
	std::atomic_size_t _curr_write_frame{0};
	std::atomic_size_t _curr_read_frame{0};
};