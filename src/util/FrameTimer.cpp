
#include "FrameTimer.hpp"
#include <chrono>
#include <thread>

void FrameTimer::start_frame() {
    auto current_time = std::chrono::steady_clock::now();
    m_delta = std::chrono::duration<float>(current_time - m_last_frame).count();
    m_last_frame = current_time;
}

float FrameTimer::delta() const {
    return m_delta;
}

void FrameTimer::cap_frame() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_frame);
    if (elapsed.count() < 16) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16 - elapsed.count()));
    }
}
