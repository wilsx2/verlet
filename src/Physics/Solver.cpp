#include "Solver.hpp"

Solver::Solver(sf::Vector2f world_size, sf::Vector2f acceleration, float radius)
{
    m_world_size = world_size;
    m_acceleration = acceleration;
    m_radius = radius;

    // Add objects to test the renderer
    m_objects.emplace_back(sf::Vector2f(10.f, 10.f), sf::Color::White);
    m_objects.emplace_back(sf::Vector2f(15.f, 12.f), sf::Color::Red);
}

const std::vector<PhysicsObject>& Solver::getObjects() const
{
    return m_objects;
}

float Solver::getRadius() const
{
    return m_radius;
}