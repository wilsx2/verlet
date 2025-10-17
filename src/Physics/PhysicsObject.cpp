#include "Physics/PhysicsObject.hpp"

PhysicsObject::PhysicsObject(sf::Vector2f position, sf::Color color)
    : PhysicsObject(position, {0.f,0.f}, color){}

PhysicsObject::PhysicsObject(sf::Vector2f position, sf::Vector2f velocity, sf::Color color)
{
    m_color = color;
    m_position = position;
    m_prev_position = position - velocity;
    m_prev_dt = 1/60.f;
}

void PhysicsObject::update(float dt, sf::Vector2f acceleration)
{
    auto velocity = (m_position - m_prev_position) * (dt / m_prev_dt);
    m_prev_position = m_position;
    m_position = m_position + velocity + acceleration * (dt * dt);
    m_prev_dt = dt;
}

void PhysicsObject::setPosition(sf::Vector2f v)
{
    m_position = v;
}

const sf::Vector2f& PhysicsObject::getPosition() const
{
    return m_position;
}

const sf::Color& PhysicsObject::getColor() const
{
    return m_color;
}