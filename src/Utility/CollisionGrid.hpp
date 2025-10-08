#ifndef LIST_GRID_H
#define LIST_GRID_H

#include <vector>
#include <unordered_set>

class CollisionGrid
{
    private:
    std::vector<std::unordered_set<std::size_t>> m_data;
    int m_width;
    int m_height;

    public:
    CollisionGrid();
    CollisionGrid(int width, int height);
    void resize(int width, int height);
    const std::unordered_set<std::size_t>& get(int x, int y) const;
    void add(int x, int y, std::size_t value);
    void clear(int x, int y);
    void clear();
};

#endif