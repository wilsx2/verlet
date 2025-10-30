#include "Physics/Solver.hpp"
#include <cmath>
#include <iostream>

Solver::Solver(ThreadPool& pool, sf::Vector2f world_size, sf::Vector2f acceleration, float radius)
    : m_pool(pool)
    , m_world_size(world_size)
    , m_acceleration(acceleration)
    , m_radius(radius)
    , m_diameter(radius * 2)
    , m_spatial_hash(m_diameter)
{
    
    m_prev_dt = 1 / 60.f;
    m_objects.reserve(65536);
}

void Solver::update(float dt)
{
    float dt_substep = dt / static_cast<float>(SUB_STEPS);
    for (int i = 0; i < SUB_STEPS; ++i)
    {
        m_pool.enqueue_for_each<sf::Vector2f>(m_objects.positions, [this, dt_substep](sf::Vector2f&, std::size_t i) {
            updateObject(i, dt_substep);
            applyConstraints(i);
        });
        m_pool.wait();
        handleCollisions();
        m_prev_dt = dt_substep;
    }
}

void Solver::updateObject(int i, float dt)
{
    auto& pos = m_objects.positions[i];
    auto& prev_pos = m_objects.prev_positions[i];

    auto velocity = (pos - prev_pos) * (dt / m_prev_dt);
    prev_pos = pos;
    pos = pos + velocity + m_acceleration * (dt * dt);
}

void Solver::applyConstraints(std::size_t i)
{
    auto& pos = m_objects.positions[i];
    if (pos.x - m_radius < 0.f)
    {
        pos.x = m_radius;
    }
    else if (pos.x + m_radius > m_world_size.x)
    {
        pos.x = m_world_size.x - m_radius;
    }

    if (pos.y - m_radius < 0.f)
    {
        pos.y = m_radius;
    }
    else if (pos.y + m_radius > m_world_size.y)
    {
        pos.y = m_world_size.y - m_radius;
    }
}

void Solver::fillSpatialHash()
{
    // Clear
    m_spatial_hash.clear();

    // Populate
    for (int i = 0; i < m_objects.size(); ++i)
    {
        auto& pos = m_objects.positions[i];
        m_spatial_hash.insert(pos.x, pos.y, i);
    } 
}

void Solver::handleCollisions()
{
    fillSpatialHash();

    for(int i = 0; i < m_objects.size(); ++i)
    {
        auto neighbors = m_spatial_hash.getNeighbors(m_objects.positions[i].x,m_objects.positions[i].y);
        for (auto& j : neighbors)
        {
            if (i != j)
                handleCollision(i, j);
        }
    }

    // for (int dx = 0; dx <= 1; dx++) {
    //     for (int dy = 0; dy <= 1; dy++) {

    //         for (float x = dx * m_diameter; x < m_world_size.x; x += m_diameter * 2)
    //         {
    //             for (float y = dy * m_diameter; y < m_world_size.y; y += m_diameter * 2)
    //             {
    //                 auto bucket = m_spatial_hash.getBucket(x, y);
    //                 if(!bucket.empty()) 
    //                     m_pool.enqueue([this, bucket](){handleCollisionsInBucket(bucket);});
    //             }
    //         }

    //         m_pool.wait();
    //     }
    // }
}

void Solver::handleCollisionsInBucket(const std::vector<std::size_t> bucket)
{
    for (auto& i : bucket)
    {
        auto neighbors = m_spatial_hash.getNeighbors(m_objects.positions[i].x,m_objects.positions[i].y);

        for (auto& j : neighbors)
        {
            if (i != j)
                handleCollision(i, j);
        }
    }
}

void Solver::handleCollision(std::size_t i, std::size_t j)
{
    auto& pos_a = m_objects.positions[i];
    auto& pos_b = m_objects.positions[j];

    sf::Vector2f difference = pos_a - pos_b;
    float dist = difference.length();
    float overlap = m_diameter - dist;
    if (overlap < 0.f)
    {
        return;
    }

    auto nudge = (difference / dist) * (overlap / 2.f);
    pos_a = pos_a + nudge;
    pos_b = pos_b - nudge;
}

void Solver::spawnObject(sf::Vector2f position, sf::Vector2f velocity, float dt)
{
    for (auto& obj_pos : m_objects.positions)
    {
        if ((obj_pos - position).length() < m_radius * 2)
        {
            return;
        }
    }

    auto previous_position = position - velocity * m_prev_dt * 60.f;
    m_objects.emplaceBack(
        position,
        previous_position
    );
}

const std::vector<sf::Vector2f>& Solver::getObjects() const
{
    return m_objects.positions;
}

float Solver::getRadius() const
{
    return m_radius;
}