#ifndef SOLVER_H
#define SOLVER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include "PhysicsObjects.hpp"
#include "Utility/SpatialHash.hpp"
#include "Utility/ThreadPool.hpp"

class Solver
{
    private:
    static constexpr int SUB_STEPS = 1;
    SpatialHash m_spatial_hash; // Logically subdivides the world into cells for parallelism
    ThreadPool& m_pool;
    PhysicsObjects m_objects;
    const sf::Vector2f m_world_size;
    const sf::Vector2f m_acceleration;
    const float m_radius;
    const float m_diameter;
    float m_prev_dt;
    void updateObject(int i, float dt);
    void fillSpatialHash();
    void handleCollisionsInBucket(const std::vector<std::size_t> bucket);
    void applyConstraints(std::size_t i); 
    void handleCollisions();
    void handleCollision(std::size_t i, std::size_t j);
    
    public:
    Solver() = default;
    Solver(ThreadPool& pool, sf::Vector2f world_size, sf::Vector2f acceleration, float radius);
    void update(float dt);
    void spawnObject(sf::Vector2f position, sf::Vector2f velocity, float dt);
    const std::vector<sf::Vector2f>& getObjects() const;
    float getRadius() const;
};

#endif