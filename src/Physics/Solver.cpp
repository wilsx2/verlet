#include "Solver.hpp"

Solver::Solver(sf::Vector2f world_size, sf::Vector2f acceleration, float radius)
{
    m_world_size = world_size;
    m_acceleration = acceleration;
    m_radius = radius;
}

void Solver::update(float dt)
{
    // Update object motion
    for (auto& obj : m_objects)
    {
        obj.update(dt, m_acceleration);
    }   
    
    apply_constraints();
    handle_collisions();
}

void Solver::apply_constraints()
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

void Solver::handle_collisions()
{
    for (auto& a : m_objects)
    {
        for (auto& b : m_objects)
        {
            if (&a != &b)
            {
                handle_collision(a,b);
            }
        }   
    }   
}

void Solver::handle_collision(PhysicsObject& a, PhysicsObject& b)
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

void Solver::spawn_object(sf::Vector2f position, sf::Vector2f velocity, sf::Color color)
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