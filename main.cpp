#include <SFML/Graphics.hpp>
#include "global.hpp"
#include <array>
#include <random>
#include <chrono>

bool isAlive(std::array<std::array<bool, SCREEN_HEIGHT/14>, SCREEN_WIDTH/14>& state, int i, int j) {
    int numberAlive = 0;
    const int neighbours[] = {-1,0,1};

    for (int dx : neighbours) {
        for (int dy : neighbours) {
            if (dx == 0 && dy == 0) continue;

            int ni = i + dx;
            int nj = j + dy;

            if (ni >= 0 && ni < state.size() && nj >=0 && nj < state[0].size()) {
                numberAlive += state[ni][nj];
            }
        }
    }

    if (state[i][j]) {
        return (numberAlive == 2 || numberAlive == 3);
    } else {
        return (numberAlive == 3);
    }

}  

void update(std::array<std::array<bool, SCREEN_HEIGHT / 14>, SCREEN_WIDTH / 14>& newState, std::array<std::array<bool, SCREEN_HEIGHT/14>, SCREEN_WIDTH/14>& currentState){
    for (int i = 0; i < SCREEN_WIDTH / 14; i++) {
        for (int j = 0; j < SCREEN_HEIGHT / 14; j++) {
            if (isAlive(currentState, i, j)){
                newState[i][j] = 1;
            } else {
                newState[i][j] = 0;
            }
        }
    }
}
 
void render(const std::array<std::array<bool, SCREEN_HEIGHT / 14>, SCREEN_WIDTH/14>& state, sf::RenderWindow& window){
    for (int i = 0; i < SCREEN_WIDTH / 14; i++) {
        for (int j = 0; j < SCREEN_HEIGHT / 14; j++) {
            if (state[i][j]) {
                sf::RectangleShape on(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                on.setFillColor(sf::Color::White);
                on.setPosition(i * 14, j * 14);
                on.setOutlineColor(sf::Color::Black);
                on.setOutlineThickness(2);
                window.draw(on);
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Game of Life");

    std::array<std::array<bool, SCREEN_HEIGHT / 14>, SCREEN_WIDTH / 14> currentState;
    std::array<std::array<bool, SCREEN_HEIGHT / 14>, SCREEN_WIDTH / 14> newState;

    //maybe i should make this less ugly
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(static_cast<unsigned>(seed));
    std::uniform_int_distribution<> dis(0, 1);

    //for when random
    // for (int i = 0; i < SCREEN_WIDTH / 14; ++i) {
    //     for (int j = 0; j < SCREEN_HEIGHT / 14; ++j) {
    //         currentState[i][j] = dis(gen) == 1; 
    //     }
    // }

    //for when manual cell placing
    for (int i = 0; i < SCREEN_WIDTH / 14; ++i) {
        for (int j = 0; j < SCREEN_HEIGHT / 14; ++j) {
            currentState[i][j] = 0; 
        }
    }

    bool gameStart = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!gameStart) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    int column = mouseX / 14;
                    int row = mouseY / 14;

                    currentState[column][row] = 1;
                    window.clear();
                    render(currentState, window);
                    window.display();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                gameStart = 1;
            }
        }

        if (gameStart){
            update(newState, currentState);
            std::swap(currentState, newState);
            newState.fill({{0}});
            window.clear();
            render(currentState, window);
            window.display();
            sf::sleep(sf::milliseconds(100));
            
        }
    }

    return 0;
}