#include <iostream>
#include <SFML/Graphics.hpp>

#include "Physics/Solver.hpp"
#include "Rendering/Renderer.hpp"

constexpr float RADIUS = 6.f;
constexpr float FRAME_TARGET = 60.f;

int main()
{
    ThreadPool pool (std::thread::hardware_concurrency());

    sf::Vector2u world_size {1600, 1000};
    Solver solver (pool, static_cast<sf::Vector2f>(world_size), {0.f, 100.f}, RADIUS);
    Renderer renderer (pool);

    sf::RenderWindow window(sf::VideoMode(world_size), "SFML Verlet");

    bool frame_target_lost = false;
    int frames_under_target = 0;
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
        /// Find delta time
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        std::cout << (1.f / dt) << " | " << solver.getObjects().size() << "\n";
        /// Spawn new objects
        if (1.f / dt < FRAME_TARGET)
        {
            frames_under_target++;
            if (frames_under_target >= 10)
            {
                frame_target_lost = true;
            }
        }
        else
        {
            frames_under_target = 0;
        }

        if (!frame_target_lost)
        {
            for (int i = 0; i < solver.getObjects().size() / 50 + 1 && i < world_size.y / RADIUS / 2 / 8; ++i)
            {
                solver.spawnObject(sf::Vector2f(RADIUS,RADIUS + i * RADIUS * 2), sf::Vector2f(6.f, 1.f), sf::Color::White);
            }
        }
        /// Update objects
        solver.update(dt);

        // Draw
        window.clear(sf::Color::Black);

        renderer.render(window, solver);

        window.display();
    }
}