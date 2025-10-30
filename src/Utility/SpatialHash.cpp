#include "Utility/SpatialHash.hpp"


SpatialHash::SpatialHash(float cell_size)
    : m_cell_size(cell_size)
{}

#include <iostream>
std::uint64_t SpatialHash::hashPosition(float x, float y)
{
    int ix = static_cast<int>(x / m_cell_size);
    int iy = static_cast<int>(y / m_cell_size);
    return (ix * 82632121) ^ (iy * 42572143);
}

void SpatialHash::insert(float x, float y, std::size_t value)
{
    m_buckets[hashPosition(x,y)].push_back(value);
}

std::vector<std::size_t> SpatialHash::getNeighbors(float x, float y)
{
    std::vector<std::size_t> neighbors;

    for(float dx = -m_cell_size; dx < +m_cell_size; dx += m_cell_size)
    {
        for(float dy = -m_cell_size; dy < +m_cell_size; dy += m_cell_size)
        {
            std::uint32_t hash = hashPosition(x + dx, y + dy);
            
            auto it = m_buckets.find(hash);
            if (it != m_buckets.end())
            {
                auto& neighbor_bucket = it->second;
                neighbors.insert(neighbors.end(), neighbor_bucket.begin(), neighbor_bucket.end());
            }
        }
    }

    return std::move(neighbors); // necessary?
}

std::vector<std::size_t>& SpatialHash::getBucket(float x, float y)
{
    static std::vector<std::size_t> empty {};
    std::uint32_t hash = hashPosition(x, y);

    auto it = m_buckets.find(hash);
    if (it != m_buckets.end())
        return it->second;
    return empty;
}

void SpatialHash::clear()
{
    m_buckets.clear();
}