#include "Physics/PhysicsObjects.hpp"


std::size_t PhysicsObjects::size()
{
    return m_positions.size();
}

void PhysicsObjects::reserve(std::size_t i) 
{
    m_positions.reserve(i);
    m_prev_positions.reserve(i);
}

sf::Vector2f& PhysicsObjects::getPosition(std::size_t i)
{
    return m_positions.at(i);
}

sf::Vector2f& PhysicsObjects::getPrevPosition(std::size_t i)
{
    return m_prev_positions.at(i);
}

std::vector<sf::Vector2f>& PhysicsObjects::getPositions()
{
    return m_positions;
}

std::vector<sf::Vector2f>& PhysicsObjects::getPrevPositions()
{
    return m_prev_positions;
}

void PhysicsObjects::emplaceBack(sf::Vector2f&& position, sf::Vector2f&& prev_position)
{
    m_positions.emplace_back(position);
    m_prev_positions.emplace_back(prev_position);
}