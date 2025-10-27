#ifndef PHYSICS_OBJECTS_H
#define PHYSICS_OBJECTS_H

#include <SFML/Graphics.hpp>
#include <vector>

class PhysicsObjects
{
    private:
    std::vector<sf::Vector2f> m_positions;
    std::vector<sf::Vector2f> m_prev_positions;

    public:
    PhysicsObjects() = default;
    void reserve(std::size_t i);
    sf::Vector2f& getPosition(std::size_t i);
    sf::Vector2f& getPrevPosition(std::size_t i);
    void emplaceBack(sf::Vector2f&& position, sf::Vector2f&& prev_position);
};

#endif