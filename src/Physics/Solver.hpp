#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include "PhysicsObject.hpp"
#include "../Utility/CollisionGrid.hpp"
#include "../Utility/ThreadPool.hpp"

class Solver
{
    private:
    CollisionGrid m_collision_grid; // Logically subdivides the world into cells for parallelism
    ThreadPool m_pool;
    std::vector<PhysicsObject> m_objects;
    sf::Vector2f m_world_size;
    sf::Vector2f m_acceleration;
    float m_radius;
    void fillGrid();
    void handleCollisionsInCell(int ix, int iy);
    void applyConstraints(PhysicsObject& obj); 
    void handleCollisions();
    void handleCollision(PhysicsObject& a, PhysicsObject& b);
    
    public:
    Solver() = default;
    Solver(sf::Vector2f world_size, sf::Vector2f acceleration, float radius);
    void update(float dt);
    void spawnObject(sf::Vector2f position, sf::Vector2f velocity, sf::Color color);
    const std::vector<PhysicsObject>& getObjects() const;
    float getRadius() const;
};

#endif