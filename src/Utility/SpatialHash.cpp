#include "Utility/SpatialHash.hpp"
#include <iostream>

SpatialHash::SpatialHash(float cell_size)
    : m_cell_size(cell_size)
{}

int SpatialHash::xToIx(float x)
{
    return static_cast<int>(x / m_cell_size);
}

int SpatialHash::yToIy(float y)
{
    return static_cast<int>(y / m_cell_size);
}

int SpatialHash::xToIx(float x)
{
    return static_cast<int>(x / m_cell_size);
}

int SpatialHash::yToIy(float y)
{
    return static_cast<int>(y / m_cell_size);
}

std::uint64_t SpatialHash::hashPosition(float x, float y)
{
    return hashPosition(
        xToIx(x),
        yToIy(y)
    );
}

std::uint64_t SpatialHash::hashPosition(int ix, int iy)
{
    return (ix * 10000) + iy;
}

int SpatialHash::colorOfPosition(int ix, int iy)
{
    return ((ix & 1) << 1) + (iy & 1);
}

int SpatialHash::colorOfPosition(int ix, int iy)
{
    return ((ix % 2) << 1) + (iy % 2); // use &?
}

void SpatialHash::insert(float x, float y, std::size_t value)
{
    int ix = xToIx(x);
    int iy = yToIy(y);
    
    std::uint64_t hash = hashPosition(ix,iy);
    int color = colorOfPosition(ix, iy);
    m_buckets[hash].push_back(value);
    m_buckets_of_color[color].emplace_back(hash);
}

std::vector<std::size_t> SpatialHash::getNeighbors(float x, float y)
{
    std::vector<std::size_t> neighbors;

    int ix = xToIx(x);
    int iy = yToIy(y);
    
    for(int dx = -1; dx <= +1; ++dx)
    {
        for(int dy = -1; dy <= +1; ++dy)
        {
            std::uint32_t hash = hashPosition(ix + dx, iy + dy);
            
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

std::vector<std::size_t>& SpatialHash::getBucket(std::uint64_t hash)
{
    // Assumes the hash is valid for performance
    return m_buckets[hash];
}

std::vector<std::uint64_t>& SpatialHash::getBucketsOfColor(int color)
{
    // Assume the index is valid for perfomance
    return m_buckets_of_color[color];
}

void SpatialHash::clear()
{
    m_buckets.clear();
    for(auto& buckets : m_buckets_of_color)
        buckets.clear();
}