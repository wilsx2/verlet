#include "Renderer.hpp"

Renderer::Renderer()
    : m_vertices(sf::PrimitiveType::Triangles)
{}

void Renderer::render(sf::RenderTarget& target, Solver& solver)
{
    auto& objects = solver.getObjects();
    m_vertices.resize(objects.size() * 6);

    for (int i = 0; i < objects.size(); ++i)
    {
        auto& pos = objects.at(i).getPosition();
        auto rad = solver.getRadius();

        int vi = i * 6;
        // first triangle
        m_vertices[vi + 0].position = pos + sf::Vector2f(-rad, -rad); // top left
        m_vertices[vi + 1].position = pos + sf::Vector2f(+rad, -rad); // top right
        m_vertices[vi + 2].position = pos + sf::Vector2f(-rad, +rad); // bottom left

        // second triangle
        m_vertices[vi + 3].position = pos + sf::Vector2f(+rad, -rad); // top right
        m_vertices[vi + 4].position = pos + sf::Vector2f(+rad, +rad); // bottom right
        m_vertices[vi + 5].position = pos + sf::Vector2f(-rad, +rad); // bottom left
    }

    target.draw(m_vertices);
}