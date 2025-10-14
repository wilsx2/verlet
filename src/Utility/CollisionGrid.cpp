#include "CollisionGrid.hpp"

CollisionGrid::CollisionGrid() = default;

CollisionGrid::CollisionGrid(int width, int height)
{
    resize(width, height);
}

int CollisionGrid::getWidth()
{
    return m_width;
}

int CollisionGrid::getHeight()
{
    return m_height;
}

void CollisionGrid::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    m_data.resize(width*height);
}

bool CollisionGrid::inBounds(int x, int y) const
{
    return x >= 0 && y >= 0 && x < m_width && y < m_height;
}

std::vector<std::size_t>& CollisionGrid::get(int x, int y)
{
    return m_data[x + y * m_width];
}

void CollisionGrid::add(int x, int y, std::size_t value)
{
    get(x,y).emplace_back(value);
}

void CollisionGrid::clear(int x, int y)
{
    get(x,y).clear();
}

void CollisionGrid::clear()
{
    m_data.resize(0);
    m_data.resize(m_width * m_height);
}