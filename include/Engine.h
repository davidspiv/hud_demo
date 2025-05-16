#pragma once

#include "Board.h"
#include "Button.h"
#include "config.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Engine {

public:
    Engine();
    void run();

private:
    sf::RenderWindow window;

    bool mouse_clicked;
    sf::Vector2i mouse_pos;

    Board board;
    Button turret_button;

    void input();
    void update();
    void draw();
};

Engine::Engine()
    : mouse_clicked(false)
    , mouse_pos(sf::Vector2i(0, 0))
    , board()
    , turret_button(Button({ SCREEN_WIDTH / 2.f - 50.f, SCREEN_HEIGHT - 150.f }))
{

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window.create(
        sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Iso Demo", sf::Style::Default, settings);

    auto const desktop = sf::VideoMode::getDesktopMode();
    window.setPosition({ static_cast<int>(desktop.width / 2 - SCREEN_WIDTH / 2),
        static_cast<int>(desktop.height / 2 - SCREEN_HEIGHT / 2) });

    window.setFramerateLimit(TARGET_FPS);

    std::cout << "Anti-Aliasing: " << (window.getSettings().antialiasingLevel ? "ON" : "OFF")
              << "\n";
}

void Engine::run()
{
    // GAMEPLAY LOOP
    while (window.isOpen()) {
        input();
        update();
        draw();
    }
}

void Engine::input()
{
    mouse_clicked = false;
    mouse_pos = sf::Mouse::getPosition(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) {
            mouse_clicked = true;
        }
    }
}

void Engine::update()
{
    board.update(mouse_pos);
    turret_button.update();
}

void Engine::draw()
{
    window.clear(sf::Color(19, 19, 19));

    for (auto& tile : board.m_tiles) {
        window.draw(tile.m_top_face);
        window.draw(tile.m_right_face);
        window.draw(tile.m_left_face);
    }

    window.draw(turret_button.shape);

    window.display();
}
