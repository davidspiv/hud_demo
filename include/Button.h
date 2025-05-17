#pragma once
#include "../lib/Signal.h"
#include "config.h"
#include "util.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

struct ButtonTheme {
    sf::Color colorBase = { 20, 20, 20 };
    sf::Color colorActive = { 50, 50, 50 };
    sf::Color colorHovered = { 200, 200, 200 };
    sf::Color colorDisabled = { 5, 5, 5 };
    float cornerRadius = 8.0f; // for rounded rect
    // Optional: font, text size, padding
};

ButtonTheme buttonTheme;

struct ButtonConfig {
    sf::Vector2f pos;
    sf::Vector2f size = { 100, 100 };
    std::string label = "";
    ButtonTheme theme = buttonTheme;
    bool isEnabled = true;
};

struct Button {

    enum DrawMode {
        NORMAL,
        HOVERED,
        CLICKED, // for one frame on mouse down
        PRESSED, // after first frame
        DISABLED,
    };

    struct State { };

    std::string label;
    Signal<void> onRelease;
    ButtonTheme theme;
    sf::Vector2f size;
    sf::Vector2f pos;
    DrawMode state;
    sf::VertexArray shape;

    Button(ButtonConfig const& config)
        : label(config.label)
        , theme(config.theme)
        , size(config.size)
        , pos(config.pos - config.size * 0.5f)
        , state(config.isEnabled ? NORMAL : DISABLED)

    {
        shape = theme.cornerRadius > 0
            ? build_primitive_rounded_rect(pos, size, theme.cornerRadius)
            : build_primitive_rounded_rect(pos, size, theme.cornerRadius);
    }

    virtual void update();
};

struct Toggle_Button : public Button {
    Signal<void> onPress;

    void update() override;
};

inline void Button::update()
{
    auto shapeColor = [&](sf::Color& color) {
        for (size_t i = 0; i < shape.getVertexCount(); i++) {
            shape[i].color = color;
        }
    };

    switch (state) {
    case NORMAL:
        shapeColor(theme.colorBase);
        break;
    case HOVERED:
        shapeColor(theme.colorHovered);
        break;
    case PRESSED:
        shapeColor(theme.colorActive);
        break;
    case DISABLED:
        shapeColor(theme.colorDisabled);
        break;
    default:
        throw std::runtime_error("unknown button state");
    }
}
