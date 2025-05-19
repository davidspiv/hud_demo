#pragma once

#include "config.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <optional>
#include <string>

using ButtonId = unsigned;

struct MouseState {
    bool justPressedLeft = false;
    bool justReleasedLeft = false;
    sf::Vector2i position;
    std::optional<ButtonId> hoveredButton;
};

inline sf::VertexArray build_rect(sf::Vector2f const pos, sf::Vector2f const size)
{
    sf::VertexArray vertices(sf::Quads);
    sf::Color const color = sf::Color::White;

    sf::Vector2f top_left = pos;
    sf::Vector2f top_right = { pos.x + size.x, pos.y };
    sf::Vector2f bottom_right = { pos.x + size.x, pos.y + size.y };
    sf::Vector2f bottom_left = { pos.x, pos.y + size.y };
    vertices.append(sf::Vertex(top_left, color));
    vertices.append(sf::Vertex(top_right, color));
    vertices.append(sf::Vertex(bottom_right, color));
    vertices.append(sf::Vertex(bottom_left, color));
    // vertices.append(vertices[0]); // close the loop

    return vertices;
}

inline sf::VertexArray build_primitive_rounded_rect(sf::Vector2f const pos, sf::Vector2f const size,
    float const radius = 20.f, std::size_t const cornerResolution = 8)
{
    sf::VertexArray vertices(sf::TriangleFan);
    sf::Color const color = sf::Color::White;

    sf::Vector2f center = pos + size * 0.5f;
    vertices.append(sf::Vertex(center, color)); // Center of fan

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
        vertices.append(sf::Vertex(pt, color));
    }
    vertices.append(vertices[1]); // close off

    return vertices;
}

void setup_window(sf::RenderWindow& window)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIAS_LEVEL;

    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Demo", sf::Style::Default, settings);

    if (!window.isOpen()) {
        throw std::runtime_error("Failed to create SFML window");
    }

    std::cout << "Anti-Aliasing: " << (window.getSettings().antialiasingLevel ? "ON" : "OFF")
              << "\n";

    auto desktop = sf::VideoMode::getDesktopMode();
    window.setPosition({ static_cast<int>(desktop.width / 2 - SCREEN_WIDTH / 2),
        static_cast<int>(desktop.height / 2 - SCREEN_HEIGHT / 2) });

    window.setFramerateLimit(TARGET_FPS);
}
