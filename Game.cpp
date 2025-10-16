#include <SFML/Graphics.hpp>
using namespace sf;

int main() {
    // Create a window of size 800×600 with title can change res later**
    RenderWindow window(VideoMode({800, 600}), "Game basis");

    // set a frame rate limit
    window.setFramerateLimit(60);

    // Create a red square (50×50 pixels)
    sf::RectangleShape square({50.f, 25.f});
    square.setFillColor(sf::Color::Red);
    square.setPosition({400.f, 540.f});  // bot-middle

    // Main Event running. most functionality in this while loop.
    while (window.isOpen()) {
        // Event handling
        window.handleEvents(
            [&](const sf::Event::Closed& ) {
                window.close();
            }
        );

        // Rendering
        window.clear(sf::Color::Black);
        window.draw(square);
        window.display();
    }

    return 0;
}
