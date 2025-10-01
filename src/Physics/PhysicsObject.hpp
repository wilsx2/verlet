#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <SFML/Graphics.hpp>

class PhysicsObject
{
    private:
    sf::Vector2f    m_position;
    sf::Vector2f    m_prev_position;
    sf::Color       m_color;

    public:
    PhysicsObject() = default;
    PhysicsObject(sf::Vector2f position, sf::Color color);
    void update(float dt, sf::Vector2f acceleration);
    void setPosition(sf::Vector2f v);
    const sf::Vector2f& getPosition() const;
    const sf::Color& getColor() const;
};

#endif