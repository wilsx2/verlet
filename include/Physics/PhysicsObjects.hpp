#ifndef PHYSICS_OBJECTS_H
#define PHYSICS_OBJECTS_H

#include <SFML/Graphics.hpp>
#include <vector>

struct PhysicsObjects
{
    std::vector<sf::Vector2f> positions;
    std::vector<sf::Vector2f> prev_positions;

    PhysicsObjects() = default;
    void reserve(std::size_t i);
    std::size_t size();
    void emplaceBack(sf::Vector2f& position, sf::Vector2f& prev_position);
};

#endif