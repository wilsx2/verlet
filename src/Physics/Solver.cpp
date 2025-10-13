#include "Solver.hpp"

Solver::Solver(sf::Vector2f world_size, sf::Vector2f acceleration, float radius)
{
    m_collision_grid = CollisionGrid(16,16);
    m_world_size = world_size;
    m_acceleration = acceleration;
    m_radius = radius;
}

void Solver::update(float dt)
{
    for (auto& obj : m_objects) // Update object motion
    {
        obj.update(dt, m_acceleration);
    }   
    
    applyConstraints();

    fillGrid();
    handleCollisions();
}

void Solver::applyConstraints()
{
    for (auto& obj : m_objects)
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
    for (int ix = 0; ix < m_collision_grid.getWidth(); ++ix)
    {
        for (int iy = 0; iy < m_collision_grid.getHeight(); ++iy)
        {
            handleCollisionsInCell(ix, iy);
        }   
    }   
}

#include <iostream>
void Solver::handleCollisionsInCell(int ix, int iy)
{
    std::set<int> object_indices {};

    // Aggregate all objects which may collide with objects in the cell
    for(int x = ix - 1; x <= ix + 1; ++x)
    {
        for(int y = iy - 1; y <= iy + 1; ++y)
        {
            if (m_collision_grid.inBounds(x,y)) {
                auto& contents = m_collision_grid.get(x,y);
                object_indices.insert(contents.begin(), contents.end());
            }
        }
    }

    // Handle collisions between objects
    for(auto i : object_indices)
    {
        for(auto j : object_indices)
        {
            if (i != j)
                handleCollision(m_objects[i],m_objects[j]);
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