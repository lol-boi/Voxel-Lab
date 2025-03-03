#pragma once
#include <chrono>

class FrameTimer {
public:
    float delta() const;
    void start_frame();
    void cap_frame() const;

private:
    std::chrono::time_point<std::chrono::steady_clock> m_last_frame;
    float m_delta = 0.0f;
};
