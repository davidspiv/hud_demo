#pragma once
#include "config.h"
#include "util.h"

#include <SFML/Graphics.hpp>
#include <cmath>

struct Button {
    sf::Vector2f size;
    sf::Vector2f pos;
    sf::VertexArray shape;
    sf::Color color_norm = sf::Color(189, 181, 155);
    sf::Color color_pressed = sf::Color(112, 107, 92);
    bool turret_selected = false;
    bool was_hovered = false;

    Button(sf::Vector2f const pos)
        : size(100.f, 100.f)
        , pos(pos)
    {
        shape = build_primitive_rounded_rect(pos, size, color_norm);
        set_fill_color(color_norm);
    }

    bool is_hovered(sf::Vector2i const& mouse_pos) const
    {
        return shape.getBounds().contains(static_cast<sf::Vector2f>(mouse_pos));
    }

    void set_fill_color(sf::Color const& color)
    {
        for (std::size_t i = 0; i < shape.getVertexCount(); ++i) {
            shape[i].color = color;
        }
    }

    void update(sf::Vector2i const& mouse_pos, bool clicked)
    {
        if (!is_hovered(mouse_pos) && clicked) {
            set_fill_color(color_norm);
            turret_selected = false;

        } else if (turret_selected || (is_hovered(mouse_pos) && clicked)) {

            set_fill_color(color_pressed);
            turret_selected = true;
            was_hovered = true;

        } else if (was_hovered) {
            set_fill_color(color_norm);
        }
    }
};
