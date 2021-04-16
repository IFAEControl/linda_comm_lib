#pragma once

#include <condition_variable>
#include <cstddef>
#include <vector>
#include <mutex>

constexpr unsigned CACHE_SIZE = 33101;

class Frame {
public:
	explicit Frame(unsigned b);

	char* get();
	void copyTo(void* dest);
	void remove();

private:
	bool _delete{false};
	char* _mem;
	unsigned _bytes;
};

class FrameBuffer {
public:
	void addFrame(const Frame&& f);
	int moveLastFrame(unsigned* data);
	void cancel();
private:
	void incWriteFrame();
	void incReadFrame();

	bool _cancel{false};

	std::vector<Frame> _buf;
	std::size_t _curr_write_frame{0};
	std::size_t _curr_read_frame{0};

	std::mutex _mutex;
	std::condition_variable _cv{};
};