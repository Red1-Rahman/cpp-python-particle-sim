#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Particle {
    float x, y;
    float vx, vy;
    float lifespan;
    float r, g, b;
};

void updateParticles(std::vector<Particle> &particles, float dt) {
    for (auto &p : particles) {
        p.vy -= 9.8f * dt;  // gravity
        p.vx += ((rand() % 100) - 50) / 500.0f; // wind
        p.x += p.vx * dt;
        p.y += p.vy * dt;
        p.lifespan -= dt;

        if (p.y < 0) { // ground collision
            p.y = 0;
            p.vy *= -0.6f;
        }
    }
    // remove dead particles
    particles.erase(std::remove_if(particles.begin(), particles.end(),
                                   [](Particle &p){ return p.lifespan <= 0; }),
                    particles.end());
}

void emitParticles(std::vector<Particle> &particles, int count, float ex, float ey) {
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.x = ex;
        p.y = ey;
        p.vx = ((rand() % 200) - 100) / 50.0f;
        p.vy = (rand() % 100) / 10.0f + 2.0f;
        p.lifespan = 5.0f;
        p.r = (rand() % 100) / 100.0f;
        p.g = (rand() % 100) / 100.0f;
        p.b = (rand() % 100) / 100.0f;
        particles.push_back(p);
    }
}

void saveParticles(const std::vector<Particle> &particles) {
    json j_particles = json::array();
    for (auto &p : particles) {
        float alpha = p.lifespan / 5.0f;
        j_particles.push_back({
            {"x", p.x}, {"y", p.y},
            {"r", p.r}, {"g", p.g}, {"b", p.b}, {"a", alpha}
        });
    }
    std::ofstream file("particles.json");
    file << j_particles.dump();
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    std::vector<Particle> particles;
    float dt = 0.05f;
    float emitterX = 0.0f, emitterY = 0.0f;
    int emitRate = 8; // particles per frame

    while (true) {
        emitParticles(particles, emitRate, emitterX, emitterY);
        updateParticles(particles, dt);
        saveParticles(particles);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}
