#ifndef LIST_GRID_H
#define LIST_GRID_H

#include <vector>
#include <set>

class CollisionGrid
{
    private:
    std::vector<std::set<std::size_t>> m_data;
    int m_width;
    int m_height;
    std::set<std::size_t>& get(int x, int y);

    public:
    CollisionGrid();
    CollisionGrid(int width, int height);
    void resize(int width, int height);
    bool inBounds(int x, int y) const;
    const std::set<std::size_t>& get(int x, int y) const;
    void add(int x, int y, std::size_t value);
    void clear(int x, int y);
    void clear();
};

#endif