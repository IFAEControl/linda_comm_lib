#include <cstring>
#include <iostream>

#include "frame_buffer.hpp"

Frame::Frame(unsigned b) : _bytes(b) {
	_mem =  new char[b];
}

char* Frame::get(){
	return _mem;
}

void Frame::copyTo(void* dest) {
	std::memcpy(dest, _mem, _bytes);
}

void Frame::remove() {
	delete[] _mem;	
}

void FrameBuffer::addFrame(const Frame&& f) {
	_mutex.lock();

	if(_curr_write_frame >= _buf.size())
		_buf.emplace_back(std::move(f));
	else
		_buf.at(_curr_write_frame) = std::move(f);

	incWriteFrame();

	if(_curr_write_frame == _curr_read_frame) {
		_buf.at(_curr_read_frame).remove();
		incReadFrame();
	}

	_mutex.unlock();
	_cv.notify_one();
}

int FrameBuffer::moveLastFrame(unsigned* data) {
	std::mutex cv_m;
    std::unique_lock<std::mutex> lk{cv_m};

	// wait for a frame	
	_cv.wait(lk);
	if(_cancel) {
		_cancel = false;
		return -1;
	}

	_mutex.lock();
	
	auto frame = _buf.at(_curr_read_frame);
	frame.copyTo(data);
	frame.remove();

	incReadFrame();

	_mutex.unlock();

	return 0;
}

void FrameBuffer::cancel() {
	_cancel = true;
	_cv.notify_one();
}


void FrameBuffer::incWriteFrame() {
	_curr_write_frame = ++_curr_write_frame % CACHE_SIZE;
}

void FrameBuffer::incReadFrame() {
	_curr_read_frame = ++_curr_read_frame % CACHE_SIZE;
}