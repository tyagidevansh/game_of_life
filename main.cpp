#include <SFML/Graphics.hpp>
#include "global.hpp"
#include <array>
#include <random>
#include <chrono>
#include <iostream>

bool isAlive(std::array<std::array<bool, SCREEN_HEIGHT/14>, SCREEN_WIDTH/14>& state, int i, int j) {
    
}   

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Game of Life");

    std::array<std::array<bool, SCREEN_HEIGHT / 14>, SCREEN_WIDTH / 14> state;

    //maybe i should make this less ugly
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned>(seed));
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < SCREEN_WIDTH / 14; ++i) {
        for (int j = 0; j < SCREEN_HEIGHT / 14; ++j) {
            state[i][j] = dis(gen) == 1; 
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(); 

        for (int i = 0; i < SCREEN_WIDTH / 14; i++) {
            for (int j = 0; j < SCREEN_HEIGHT / 14; j++) {
                if (state[i][j]) { 
                    sf::RectangleShape on(sf::Vector2f(10, 10));
                    on.setFillColor(sf::Color::White);
                    on.setPosition(i * 14, j * 14);
                    on.setOutlineColor(sf::Color::Black);
                    on.setOutlineThickness(2);
                    window.draw(on);
                } else {
                    sf::RectangleShape off(sf::Vector2f(10, 10));
                    off.setFillColor(sf::Color::Black);
                    off.setPosition(i * 14, j * 14);
                    off.setOutlineColor(sf::Color::Black);
                    off.setOutlineThickness(2);
                    window.draw(off);
                }
            }
        }

        window.display(); 
    }

    return 0;
}