#include "Utility/SpatialHash.hpp"


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

std::uint64_t SpatialHash::hashPosition(float x, float y)
{
    return hashPosition(
        xToIx(x),
        yToIy(y)
    );
}

std::uint64_t SpatialHash::hashPosition(int ix, int iy)
{
    return (ix * 82632121) ^ (iy * 42572143);
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

std::vector<std::size_t>& SpatialHash::getBucket(std::uint64_t hash)
{
    // Assumes that the bucket exists, will result in an error if not true.
    return m_buckets[hash];
}

void SpatialHash::clear()
{
    m_buckets.clear();
}