#include "Physics/PhysicsObjects.hpp"


std::size_t PhysicsObjects::size()
{
    return positions.size();
}

void PhysicsObjects::reserve(std::size_t i) 
{
    positions.reserve(i);
    prev_positions.reserve(i);
}

void PhysicsObjects::emplaceBack(sf::Vector2f& position, sf::Vector2f& prev_position)
{
    positions.emplace_back(position);
    prev_positions.emplace_back(prev_position);
}