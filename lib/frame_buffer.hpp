#pragma once

#include <condition_variable>
#include <cstddef>
#include <vector>
#include <mutex>
#include <atomic>

constexpr unsigned CACHE_SIZE = 33101;

class Frame {
public:
	explicit Frame(unsigned b);

	char* get();
	unsigned getBytes() const;
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
	void reset();

	std::size_t getWriteFrame() const;
	std::size_t getReadFrame() const;
	std::size_t currFrames() const;
private:
	void incWriteFrame();
	void incReadFrame();

	bool _cancel{false};

	std::vector<Frame> _buf;
	std::size_t _curr_write_frame{0};
	std::size_t _curr_read_frame{0};
	std::atomic_size_t _available_frames{0};

	std::mutex _mutex;
	std::condition_variable _cv{};
};