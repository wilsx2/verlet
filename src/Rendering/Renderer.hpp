#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "../Physics/Solver.hpp"
#include "../Utility/ThreadPool.hpp"

class Renderer
{
    private:
    sf::VertexArray m_vertices;
    ThreadPool m_pool;

    public:
    Renderer();
    void render(sf::RenderTarget& target, Solver& solver);
};

#endif