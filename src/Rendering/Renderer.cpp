#include "Renderer.hpp"

void Renderer::render(sf::RenderTarget& target, Solver& solver)
{
    m_circle.setRadius(solver.getRadius());
    m_circle.setOrigin({solver.getRadius(), solver.getRadius()});
    for (const PhysicsObject& obj : solver.getObjects())
    {
        m_circle.setPosition(obj.getPosition());
        m_circle.setFillColor(obj.getColor());
        target.draw(m_circle);
    }
}