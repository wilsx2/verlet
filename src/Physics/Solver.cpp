#include "Physics/Solver.hpp"
#include <cmath>
#include <iostream>

Solver::Solver(ThreadPool& pool, sf::Vector2f world_size, sf::Vector2f acceleration, float radius)
    : m_pool(pool)
{
    m_collision_grid = CollisionGrid(32,32);
    m_world_size = world_size;
    m_acceleration = acceleration;
    m_radius = radius;

    m_objects.reserve(65536);
}

void Solver::update(float dt)
{
    float dt_substep = dt / static_cast<float>(SUB_STEPS);
    for (int i = 0; i < SUB_STEPS; ++i)
    {
        m_pool.enqueue_for_each<PhysicsObject>(m_objects, [this, dt_substep](PhysicsObject& object, std::size_t) {
            object.update(dt_substep, m_acceleration);
            applyConstraints(object);

        });
        m_pool.wait();

        handleCollisions();
    }
}

void Solver::applyConstraints(PhysicsObject& obj)
{
    auto& pos = obj.getPosition(); 
    if (pos.x - m_radius < 0.f)
    {
        obj.setPosition({m_radius, pos.y});
    }
    else if (pos.x + m_radius > m_world_size.x)
    {
        obj.setPosition({m_world_size.x - m_radius, pos.y});
    }

    if (pos.y - m_radius < 0.f)
    {
        obj.setPosition({pos.x, m_radius});
    }
    else if (pos.y + m_radius > m_world_size.y)
    {
        obj.setPosition({pos.x, m_world_size.y - m_radius});
    }
}

void Solver::fillGrid()
{
    m_collision_grid.clear();
    for (int i = 0; i < m_objects.size(); ++i)
    {
        auto& pos = m_objects[i].getPosition();
        int ix = (pos.x / m_world_size.x)
            * static_cast<float>(m_collision_grid.getWidth());
        int iy = (pos.y / m_world_size.y)
            * static_cast<float>(m_collision_grid.getHeight());

        if (m_collision_grid.inBounds(ix,iy))
            m_collision_grid.add(ix, iy, i);
    } 
}

void Solver::handleCollisions()
{
    fillGrid();

    for (int dx = 0; dx <= 1; dx++) {
        for (int dy = 0; dy <= 1; dy++) {

            for (int ix = dx; ix < m_collision_grid.getWidth(); ix += 2)
            {
                for (int iy = dy; iy < m_collision_grid.getHeight(); iy += 2)
                {
                    m_pool.enqueue([this, ix, iy](){handleCollisionsInCell(ix, iy);});
                }
            }

            m_pool.wait();
        }
    }
}

void Solver::handleCollisionsInCell(int ix, int iy)
{
    auto& objects = m_collision_grid.get(ix,iy);

    // Aggregate all objects which may collide with objects in the cell
    for(auto& object : objects)
    {
        for (int x = ix - 1; x <= ix + 1; ++x)
        {
            for (int y = iy - 1; y <= iy + 1; ++y)
            {
                if (m_collision_grid.inBounds(x,y))
                {
                    auto& others = m_collision_grid.get(x,y);
                    for(auto& other : others)
                    {
                        if(other != object)
                        handleCollision(m_objects[object], m_objects[other]);
                    }
                }
            }
        }
    }
}

void Solver::handleCollision(PhysicsObject& a, PhysicsObject& b)
{
    auto difference = a.getPosition() - b.getPosition();
    auto overlap = (m_radius * 2) - difference.length();
    if (overlap < 0.f)
    {
        return;
    }

    auto nudge = difference.normalized() * (overlap / 2.f);
    a.setPosition(a.getPosition() + nudge);
    b.setPosition(b.getPosition() - nudge);
}

void Solver::spawnObject(sf::Vector2f position, sf::Vector2f velocity, sf::Color color)
{
    for (auto& obj : m_objects)
    {
        if ((obj.getPosition() - position).length() < m_radius * 2)
        {
            return;
        }
    }

    m_objects.emplace_back(position, velocity, color);
}

const std::vector<PhysicsObject>& Solver::getObjects() const
{
    return m_objects;
}

float Solver::getRadius() const
{
    return m_radius;
}