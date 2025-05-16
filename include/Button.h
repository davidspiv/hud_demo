#pragma once
#include "config.h"
#include "util.h"

#include <SFML/Graphics.hpp>
#include <cmath>

struct Button {

    enum ButtonState { NORMAL, HOVERED, PRESSED };

    sf::Vector2f size;
    sf::Vector2f pos;
    ButtonState state;
    sf::VertexArray shape;

    Button(sf::Vector2f const pos)
        : size(100.f, 100.f)
        , pos({ pos.x - size.x * 0.5f, pos.y - size.y * 0.5f })
        , state(NORMAL)
        , shape(build_primitive_rounded_rect(pos, size))
    {
    }

    void update();
};

inline void Button::update()
{
    sf::Color newColor;

    switch (state) {
    case NORMAL:
        newColor = sf::Color(10, 10, 10);
        break;
    case HOVERED:
        newColor = sf::Color(50, 50, 50);
        break;
    case PRESSED:
        newColor = sf::Color(200, 200, 200);
        break;

    default:
        throw std::runtime_error("unknown button state");
    }

    for (size_t i = 0; i < shape.getVertexCount(); i++) {
        shape[i].color = newColor;
    }
}
