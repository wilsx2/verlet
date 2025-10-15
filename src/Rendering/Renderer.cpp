#include "Renderer.hpp"

Renderer::Renderer()
    : m_vertices(sf::PrimitiveType::Triangles)
{
    auto _ = m_texture.loadFromFile("res/kirby.png");
    auto _ = m_texture.generateMipmap();
    m_texture.setSmooth(true);
}

void Renderer::render(sf::RenderTarget& target, Solver& solver)
{
    auto& objects = solver.getObjects();
    m_vertices.resize(objects.size() * 6);

    int threads = m_pool.size();
    int slice_size = ((objects.size() + threads - 1) / threads);
    if(slice_size <= 0)
        return;

    for (int i = 0; i < objects.size(); i += slice_size) // Update object motion
    {
        auto rad = solver.getRadius();
        sf::Vector2f tex_size = static_cast<sf::Vector2f>(m_texture.getSize());
        m_pool.enqueue([this, objects, rad, slice_size, i, tex_size](){
            for (int i = 0; i < objects.size(); ++i)
            {
                auto& pos = objects.at(i).getPosition();
                
                int vi = i * 6;
                // first triangle
                m_vertices[vi + 0].position = pos + sf::Vector2f(-rad, -rad); // top left
                m_vertices[vi + 0].texCoords = {0.f, 0.f};
                m_vertices[vi + 1].position = pos + sf::Vector2f(+rad, -rad); // top right
                m_vertices[vi + 1].texCoords = {tex_size.x, 0.f};
                m_vertices[vi + 2].position = pos + sf::Vector2f(-rad, +rad); // bottom left
                m_vertices[vi + 2].texCoords = {0.f, tex_size.y};

                // second triangle
                m_vertices[vi + 3].position = pos + sf::Vector2f(+rad, -rad); // top right
                m_vertices[vi + 3].texCoords = {tex_size.x, 0.f};
                m_vertices[vi + 4].position = pos + sf::Vector2f(+rad, +rad); // bottom right
                m_vertices[vi + 4].texCoords = {tex_size.x, tex_size.y};
                m_vertices[vi + 5].position = pos + sf::Vector2f(-rad, +rad); // bottom left
                m_vertices[vi + 5].texCoords = {0.f, tex_size.y};
            }
        });
    }
    m_pool.wait();
    
    sf::RenderStates states;
    states.texture = &m_texture;
    target.draw(m_vertices, states);
}