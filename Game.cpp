#include <SFML/Graphics.hpp>
using namespace sf;

//note all functionality is in main() functiion currently this will need to be fixed later
int main() { 
    // Create a window of size 800×600 with title can change res later**
    RenderWindow window(VideoMode({800, 600}), "Game basis");

    // set a frame rate limit
    window.setFramerateLimit(60);

    // Create a red square (50×50 pixels)
    RectangleShape character({50.f, 25.f});
    //Texture texture("Enemy.png", false, IntRect({15,15}, {100, 100}));
    //Sprite Enemy(texture);
    character.setFillColor(Color::Cyan);
    character.setPosition({400.f, 540.f});  // bot-middle

    // Main Event running. most functionality in this while loop.
    while (window.isOpen()) {
        // Event handling
        window.handleEvents(
            [&](const Event::Closed& ) {
                window.close();
            }
        );
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            character.move({-7.f, 0.f}); // left key is pressed: move our character
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
            character.move({7.f, 0.f}); // right key is pressed: move our character
        }
        // Rendering
        window.clear(Color::Black);
        window.draw(character);
        //window.draw(Enemy);
        window.display();
    }

    return 0;
}
