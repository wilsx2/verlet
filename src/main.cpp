#include <iostream>
#include <SFML/Graphics.hpp>

#include "Physics/Solver.hpp"
#include "Rendering/Renderer.hpp"

using Clock = std::chrono::steady_clock;

int main()
{
    sf::Vector2u world_size {1000, 1000};
    Solver solver (static_cast<sf::Vector2f>(world_size), {0.f, 30.f}, 30.f);
    Renderer renderer {};

    sf::RenderWindow window(sf::VideoMode(world_size), "SFML Verlet");

    sf::Clock clock;
    while (window.isOpen())
    {
        // Inputs
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Processing
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        solver.update(dt);


        // Draw
        window.clear(sf::Color::Black);

        renderer.render(window, solver);

        window.display();
    }
}