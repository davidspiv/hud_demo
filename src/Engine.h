#pragma once

#include "Button.h"
#include "util.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Engine {

public:
    Engine();
    void run();

private:
    sf::RenderWindow m_window;
    MouseState mouseState;
    std::vector<Button> m_buttons;

    void input();
    void update();
    void draw();
};

Engine::Engine()
    : mouseState({ false, false, { 0, 0 }, 0 })
{
    setup_window(m_window);
}

void Engine::input()
{
    // Update mouse position
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    mouseState.position = static_cast<sf::Vector2i>(m_window.mapPixelToCoords(pixelPos));

    // Reset per-frame button state
    mouseState.justPressedLeft = false;
    mouseState.justReleasedLeft = false;

    // Process window events
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) {
            mouseState.justPressedLeft = true;
        }

        if (event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Left) {
            mouseState.justReleasedLeft = true;
        }
    }

    for (auto& button : m_buttons) {

        if (button.shape.getBounds().contains(static_cast<sf::Vector2f>(mouseState.position))) {
            mouseState.hoveredButton = button.id;
            break;
        }

        mouseState.hoveredButton = {};
    }

    // Keyboard shortcut to quit
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        m_window.close();
    }
}

void Engine::update()
{
    for (auto& button : m_buttons) {
        button.update(mouseState);
    }
}

void Engine::draw()
{
    m_window.clear(sf::Color(19, 19, 19));

    for (auto& button : m_buttons) {
        m_window.draw(button.shape);
    }

    m_window.display();
}

void clickTest() { std::cout << "CLICKED" << std::endl; }

void Engine::run()
{
    Button button({ 100, 100 }, "hello", { 100, 100 });
    button.pressed.connect(&clickTest);
    m_buttons.emplace_back(button);

    while (m_window.isOpen()) {
        input();
        update();
        draw();
    }
}
