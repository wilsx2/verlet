#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "../Physics/Solver.hpp"

class Renderer
{
    private:
    sf::CircleShape m_circle;

    public:
    Renderer() = default;
    void render(sf::RenderTarget& target, Solver& solver);
};

#endif