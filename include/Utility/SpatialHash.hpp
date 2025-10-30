#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <unordered_map>
#include <vector>
#include <optional>
#include <cstdint>

class SpatialHash
{
    private:
    std::unordered_map<std::uint64_t, std::vector<std::size_t>> m_buckets;
    const float m_cell_size;

    std::uint64_t hashPosition(float x, float y);

    public:
    SpatialHash(float cell_size);
    void insert(float x, float y, std::size_t value);
    std::vector<std::size_t> getNeighbors(float x, float y);
    std::vector<std::size_t>& getBucket(float x, float y);
    void clear();
};

#endif