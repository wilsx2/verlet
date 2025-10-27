#include "Rendering/Renderer.hpp"

Renderer::Renderer(ThreadPool& pool)
    : m_pool(pool)
    , m_vertices(sf::PrimitiveType::Triangles)
{
    auto _ = m_texture.loadFromFile("res/kirby.png");
    auto _ = m_texture.generateMipmap();
    m_texture.setSmooth(true);
    m_tex_size = static_cast<sf::Vector2f>(m_texture.getSize());
}

#include <iostream>
void Renderer::render(sf::RenderTarget& target, Solver& solver)
{
    auto& objects = solver.getObjects();
    m_vertices.resize(objects.size() * 6);
    
    auto rad = solver.getRadius();
    m_pool.enqueue_for_each<sf::Vector2f>(objects,
        [&](const sf::Vector2f& pos, std::size_t i) {
            int vi = i * 6;
            // first triangle
            m_vertices[vi + 0].position = pos + sf::Vector2f(-rad, -rad); // top left
            m_vertices[vi + 0].texCoords = {0.f, 0.f};
            m_vertices[vi + 1].position = pos + sf::Vector2f(+rad, -rad); // top right
            m_vertices[vi + 1].texCoords = {m_tex_size.x, 0.f};
            m_vertices[vi + 2].position = pos + sf::Vector2f(-rad, +rad); // bottom left
            m_vertices[vi + 2].texCoords = {0.f, m_tex_size.y};

            // second triangle
            m_vertices[vi + 3].position = pos + sf::Vector2f(+rad, -rad); // top right
            m_vertices[vi + 3].texCoords = {m_tex_size.x, 0.f};
            m_vertices[vi + 4].position = pos + sf::Vector2f(+rad, +rad); // bottom right
            m_vertices[vi + 4].texCoords = {m_tex_size.x, m_tex_size.y};
            m_vertices[vi + 5].position = pos + sf::Vector2f(-rad, +rad); // bottom left
            m_vertices[vi + 5].texCoords = {0.f, m_tex_size.y};
        }
    );
    m_pool.wait();
    
    sf::RenderStates states;
    states.texture = &m_texture;
    target.draw(m_vertices, states);
}