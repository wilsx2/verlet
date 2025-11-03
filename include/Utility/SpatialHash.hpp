#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <array>
#include <array>
#include <unordered_map>
#include <vector>
#include <optional>
#include <cstdint>

class SpatialHash
{
    private:
    std::unordered_map<std::uint64_t, std::vector<std::size_t>> m_buckets;
    std::array<std::vector<std::uint64_t>, 4> m_buckets_of_color;
    std::array<std::vector<std::uint64_t>, 4> m_buckets_of_color;
    const float m_cell_size;

    int xToIx(float x);
    int yToIy(float y);
    std::uint64_t hashPosition(int ix, int iy);
    int xToIx(float x);
    int yToIy(float y);
    std::uint64_t hashPosition(int ix, int iy);
    std::uint64_t hashPosition(float x, float y);
    int colorOfPosition(int ix, int iy);
    int colorOfPosition(int ix, int iy);

    public:
    SpatialHash(float cell_size);
    void insert(float x, float y, std::size_t value);
    std::vector<std::size_t> getNeighbors(float x, float y);
    std::vector<std::uint64_t>& getBucketsOfColor(int color);
    std::vector<std::size_t>& getBucket(std::uint64_t hash);
    void clear();
};

#endif