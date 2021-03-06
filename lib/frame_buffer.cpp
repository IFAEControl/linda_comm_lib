#include <chrono>
#include <cstring>
#include <iostream>

#include "frame_buffer.hpp"
#include "log.hpp"

Frame::Frame(unsigned b) : _bytes(b) {
    _mem = new char[b];
}

char* Frame::get() {
    return _mem;
}

unsigned Frame::getBytes() const {
    return _bytes;
}

void Frame::copyTo(void* dest) {
    std::memcpy(dest, _mem, _bytes);
}

void Frame::remove() {
    if(_mem != nullptr) {
        delete[] _mem;
        _mem = nullptr;
    }
}

void FrameBuffer::addFrame(const Frame&& f) {
    logger->debug("Adding new frame to buffer");
    _mutex.lock();

    if(_curr_write_frame >= _buf.size()) _buf.emplace_back(std::move(f));
    else
        _buf.at(_curr_write_frame) = std::move(f);

    incWriteFrame();

    if(_curr_write_frame == _curr_read_frame) {
        _buf.at(_curr_read_frame).remove();
        incReadFrame();
    }

    _available_frames++;
    _cv.notify_one();
    _mutex.unlock();
}

int FrameBuffer::moveLastFrame(unsigned* data, unsigned ms) {
    logger->debug("Waiting for available frames");
    unsigned bytes = 0;
    std::mutex cv_m;
    std::unique_lock<std::mutex> lk{cv_m};

    // wait for a frame
    if(ms == 0) {
        _cv.wait(lk, [&] { return _cancel || _available_frames > 0; });
    } else {
        auto ret = _cv.wait_for(lk, std::chrono::milliseconds(ms),
                                [&] { return _cancel || _available_frames > 0; });
        if(!ret) return -2;
    }

    if(_cancel) {
        _cancel = false;
        return -1;
    }

    logger->debug("Retrieving frame");

    _mutex.lock();

    auto& frame = _buf.at(_curr_read_frame);
    frame.copyTo(data);
    bytes = frame.getBytes();
    frame.remove();

    incReadFrame();

    _available_frames--;
    _mutex.unlock();

    return bytes;
}

void FrameBuffer::cancel() {
    _cancel = true;
    _cv.notify_one();
}

void FrameBuffer::reset() {
    _mutex.lock();
    for(auto&& e : _buf) e.remove();
    _buf.clear();
    _curr_write_frame = 0;
    _curr_read_frame = 0;
    _available_frames = 0;
    _mutex.unlock();
}

std::size_t FrameBuffer::getWriteFrame() const {
    return _curr_write_frame;
}

std::size_t FrameBuffer::getReadFrame() const {
    return _curr_read_frame;
}

std::size_t FrameBuffer::currFrames() const {
    return _available_frames;
}

void FrameBuffer::incWriteFrame() {
    ++_curr_write_frame;
    _curr_write_frame %= CACHE_SIZE;
}

void FrameBuffer::incReadFrame() {
    ++_curr_read_frame;
    _curr_read_frame %= CACHE_SIZE;
}
