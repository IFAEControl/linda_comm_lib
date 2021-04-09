#pragma once

#include <cstddef>

constexpr unsigned CACHE_SIZE = 33101;

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
	std::size_t _curr_write_frame{0};
	std::size_t _curr_read_frame{0};
};