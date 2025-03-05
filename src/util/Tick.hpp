// Tick.hpp
#pragma once
#include <thread>
#include <atomic>
#include <glm/glm.hpp>
#include "../terrain/Terrain.hpp"

class Tick {
public:
    Tick(Terrain* terrain);
    ~Tick();

    void start();
    void stop();
    void updateCameraPosition(const glm::vec3& position);

private:
    void run();

    Terrain* terrain;
    std::atomic<bool> running;
    std::thread thread;
    glm::vec3 camera_position;
};
