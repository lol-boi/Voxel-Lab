#include "Tick.hpp"
#include <chrono>
#include <memory>

Tick::Tick(Terrain* terrain)
    : terrain(terrain), running(false) {}

Tick::~Tick() {
    stop();
}

void Tick::start() {
    if (running) return;
    running = true;
    thread = std::thread(&Tick::run, this);
}

void Tick::stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void Tick::updateCameraPosition(const glm::vec3& position) {
    camera_position = position;
}

void Tick::run() {
    const std::chrono::milliseconds interval(50); // 20 ticks/second

    while (running) {
        auto start = std::chrono::steady_clock::now();

        // Get current camera position
        glm::vec3 currentCamPos;
        {
            currentCamPos = camera_position;
        }

        // Update terrain based on camera position
        terrain->init_world_chunks(currentCamPos);

        // Maintain consistent tick rate
        auto end = std::chrono::steady_clock::now();
        auto elapsed = end - start;
        if (elapsed < interval) {
            std::this_thread::sleep_for(interval - elapsed);
        }
    }
}
