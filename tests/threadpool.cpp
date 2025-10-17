#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utility/ThreadPool.hpp"

void count_to_one_million()
{
    for(int i = 0; i < 1000000; ++i) { }
}

int main()
{
    ThreadPool pool (std::thread::hardware_concurrency());

    sf::Clock clock;
    for (int i = 0; i < 1000000; ++i)
    {
        pool.enqueue(count_to_one_million);
    }
    pool.wait();

    std::cout << "Took " << clock.reset().asSeconds() << " seconds to count to one million one million times.\n";
    
}