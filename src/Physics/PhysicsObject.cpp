#include "PhysicsObject.hpp"

PhysicsObject::PhysicsObject(sf::Vector2f position, sf::Color color)
    : PhysicsObject(position, {0.f,0.f}, color){}

PhysicsObject::PhysicsObject(sf::Vector2f position, sf::Vector2f velocity, sf::Color color)
{
    m_color = color;
    m_position = position;
    m_prev_position = position - velocity;
}

void PhysicsObject::update(float dt, sf::Vector2f acceleration)
{
    auto displacement = m_position - m_prev_position;
    m_prev_position = m_position;
    m_position = m_position + displacement + acceleration * (dt * dt);
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