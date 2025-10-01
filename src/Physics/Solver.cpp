#include "Solver.hpp"

Solver::Solver(sf::Vector2f world_size, sf::Vector2f acceleration, float radius)
{
    m_world_size = world_size;
    m_acceleration = acceleration;
    m_radius = radius;

    // Add objects to test the renderer
    m_objects.emplace_back(sf::Vector2f(50.f, 50.f), sf::Color::White);
    m_objects.emplace_back(sf::Vector2f(60.f, 70.f), sf::Color::Red);
}

void Solver::update(float dt)
{
    // Move objects
    for (auto& obj : m_objects)
    {
        obj.update(dt, m_acceleration);
    }

    // Constrain to world
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

const std::vector<PhysicsObject>& Solver::getObjects() const
{
    return m_objects;
}

float Solver::getRadius() const
{
    return m_radius;
}