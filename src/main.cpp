#include <SFML/Graphics.hpp>

#include "Physics/Solver.hpp"
#include "Rendering/Renderer.hpp"

int main()
{
    sf::Vector2u world_size {1000, 1000};
    Solver solver (static_cast<sf::Vector2f>(world_size), {0.f, 1.f}, 5.f);
    Renderer renderer {};

    sf::RenderWindow window(sf::VideoMode(world_size), "SFML Verlet");
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        renderer.render(window, solver);

        window.display();
    }
}