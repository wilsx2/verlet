#ifndef SOLVER_H
#define SOLVER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "PhysicsObjects.hpp"
#include "Utility/CollisionGrid.hpp"
#include "Utility/ThreadPool.hpp"

class Solver
{
    private:
    static constexpr int SUB_STEPS = 2;
    CollisionGrid m_collision_grid; // Logically subdivides the world into cells for parallelism
    ThreadPool& m_pool;
    PhysicsObjects m_objects;
    sf::Vector2f m_world_size;
    sf::Vector2f m_acceleration;
    float m_radius;
    float m_diameter;
    float m_prev_dt;
    void updateObject(int i, float dt);
    void fillGrid();
    void handleCollisionsInCell(int ix, int iy);
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