#include "PhysicsObject.hpp"

PhysicsObject::PhysicsObject(sf::Vector2f position, sf::Color color)
{
    m_color = color;
    m_position = position;
}

const sf::Vector2f& PhysicsObject::getPosition() const
{
    return m_position;
}

const sf::Color& PhysicsObject::getColor() const
{
    return m_color;
}