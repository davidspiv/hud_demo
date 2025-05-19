#pragma once
#include "Signal.h"
#include "config.h"
#include "util.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

struct ButtonTheme {
    sf::Color colorUnpressed;
    sf::Color colorPressed;
    sf::Color colorHover;
    float cornerRadius;
};

struct ButtonConfig {
    ButtonTheme theme;
    sf::Vector2f center;
    sf::Vector2f size;
    std::string label;
};

class Button {

    struct State {
        bool disabled = false;
        bool hovering = false;
        bool justPressed = false;
        bool pressed = false;
    } state;

public:
    static ButtonTheme defaultTheme;
    static ButtonConfig defaultConfig;
    static ButtonId nextId;

    ButtonId id;
    std::string label;
    ButtonTheme theme;
    sf::Vector2f size;
    sf::Vector2f pos;
    sf::VertexArray shape;

    Signal<void> pressed;
    Signal<void> released;

    Button(sf::Vector2f const& center, std::string const& label, sf::Vector2f const& size);
    Button(ButtonConfig const& config);

    void update(MouseState const& mouse);

private:
    void buildShape();
    void changeColor(sf::Color const& color);
};

ButtonTheme Button::defaultTheme = {
    sf::Color(50, 50, 50),    // color ready
    sf::Color(200, 200, 200), // color pressed
    sf::Color(100, 100, 100), // color hovered
    20.f,                     // corner radius
};

ButtonConfig Button::defaultConfig = {
    Button::defaultTheme,       //
    sf::Vector2f(100.f, 100.f), // center dim
    sf::Vector2f(100.f, 100.f), // size dim
    "",                         // label
};

ButtonId Button::nextId = 0;

void Button::buildShape()
{
    shape = theme.cornerRadius > 0 ? build_primitive_rounded_rect(pos, size, theme.cornerRadius)
                                   : build_rect(pos, size);

    changeColor(theme.colorUnpressed);
}

void Button::changeColor(sf::Color const& color)
{
    for (size_t i = 0; i < shape.getVertexCount(); ++i) {
        shape[i].color = color;
    }
}

inline Button::Button(ButtonConfig const& config)
    : id(nextId)
    , label(config.label)
    , theme(config.theme)
    , size(config.size)
    , pos(config.center - 0.5f * size) // top left corner

{
    buildShape();
    ++nextId;
}

Button::Button(sf::Vector2f const& center, std::string const& label, sf::Vector2f const& size)
    : Button(ButtonConfig { defaultTheme, center, size, label })
{
}

void Button::update(MouseState const& mouse)
{
    bool isHovered = (mouse.hoveredButton == id);
    bool justPressed = isHovered && mouse.justPressedLeft;
    bool justReleased = mouse.justReleasedLeft;

    if (justPressed) {
        state.pressed = true;
        changeColor(theme.colorPressed);
        pressed.emit();
    }

    if (justReleased && state.pressed) {
        state.pressed = false;
        changeColor(isHovered ? theme.colorHover : theme.colorUnpressed);
        released.emit();
    }

    if (!state.pressed && isHovered) {
        changeColor(theme.colorHover);
    } else if (!state.pressed) {
        changeColor(theme.colorUnpressed);
    }
}
