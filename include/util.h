#pragma once

#include "config.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

inline float calc_dist(sf::Vector2f const coord_a, sf::Vector2f const coord_b)
{
    sf::Vector2f mag = coord_b - coord_a;
    return std::sqrt(mag.x * mag.x + mag.y * mag.y);
}

inline sf::CircleShape build_circle()
{
    sf::CircleShape circle(12.f);
    circle.setFillColor(sf::Color(75, 68, 58));
    circle.setOrigin(circle.getRadius(), circle.getRadius());

    return circle;
}

inline sf::CircleShape build_circle(sf::Vector2f const pos, sf::Color color, float radius)
{
    sf::CircleShape shape(radius);
    shape.setPosition(pos.x - radius, pos.y - radius);
    shape.setFillColor(color);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    return shape;
}

inline sf::VertexArray build_primitive_rounded_rect(sf::Vector2f const pos, sf::Vector2f const size,
    float const radius = 20.f, std::size_t const cornerResolution = 8)
{
    sf::Color const buttonColor = sf::Color::White;

    sf::VertexArray vertices(sf::TriangleFan);

    sf::Vector2f center = pos + size * 0.5f;
    vertices.append(sf::Vertex(center, buttonColor)); // Center of fan

    std::vector<sf::Vector2f> arcPoints;

    // Generate arc points for each corner
    auto arc = [&](sf::Vector2f corner_foc, float startAngleRad) {
        for (std::size_t i = 0; i <= cornerResolution; ++i) {
            float const angle
                = startAngleRad + M_PI / 2.f * static_cast<float>(i) / cornerResolution;
            float const x = corner_foc.x + std::cos(angle) * radius;
            float const y = corner_foc.y + std::sin(angle) * radius;
            arcPoints.emplace_back(x, y);
        }
    };

    // Generate corner arcs clockwise
    arc(pos + sf::Vector2f(radius, radius), M_PI);                  // Top-left
    arc(pos + sf::Vector2f(size.x - radius, radius), 1.5f * M_PI);  // Top-right
    arc(pos + sf::Vector2f(size.x - radius, size.y - radius), 0.f); // Bottom-right
    arc(pos + sf::Vector2f(radius, size.y - radius), 0.5f * M_PI);  // Bottom-left

    // Append arc points to vertex array
    for (auto const& pt : arcPoints) {
        vertices.append(sf::Vertex(pt, buttonColor));
    }
    vertices.append(vertices[1]); // close off turret

    return vertices;
}

inline sf::ConvexShape create_top_face(float size)
{
    sf::ConvexShape shape(4);
    float half = size / 2.f;
    shape.setPoint(0, { 0.f, -half });
    shape.setPoint(1, { size, 0.f });
    shape.setPoint(2, { 0.f, half });
    shape.setPoint(3, { -size, 0.f });

    shape.setFillColor(sf::Color(52, 95, 60));
    shape.setOutlineThickness(-2.f);
    shape.setOutlineColor(sf::Color(93, 171, 108));

    return shape;
}

inline sf::ConvexShape create_right_face(float size)
{
    sf::ConvexShape shape(4);
    float w = size;
    float h = size / 2.f;
    float z = size;

    shape.setPoint(0, { w, 0.f });
    shape.setPoint(1, { 0.f, h });
    shape.setPoint(2, { 0.f, h + z });
    shape.setPoint(3, { w, z });

    shape.setFillColor(sf::Color(78, 46, 25));
    shape.setOutlineThickness(-2.f);
    shape.setOutlineColor(sf::Color(154, 91, 49));

    return shape;
}

inline sf::ConvexShape create_left_face(float size)
{
    sf::ConvexShape shape(4);
    float w = size;
    float h = size / 2.f;
    float z = size;

    shape.setPoint(0, { -w, 0.f });
    shape.setPoint(1, { 0.f, h });
    shape.setPoint(2, { 0.f, h + z });
    shape.setPoint(3, { -w, z });

    shape.setFillColor(sf::Color(139, 99, 64));
    shape.setOutlineThickness(-2.f);
    shape.setOutlineColor(sf::Color(215, 153, 99));

    return shape;
}
