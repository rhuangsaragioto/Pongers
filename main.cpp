#include "Pong.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

int main(int argc, const char* argv[]) {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Pongers");
    window.setFramerateLimit(60);

    sf::CircleShape circle(10);

    sf::SoundBuffer paddle_buffer;
    sf::SoundBuffer wall_buffer;
    if (!paddle_buffer.loadFromFile("assets/sounds/paddle_hit.wav")) {
        std::cerr << "Failed to open .wav file." << std::endl;
        return -1;
    }
    if (!wall_buffer.loadFromFile("assets/sounds/wall_hit.wav")) {
        std::cerr << "Failed to open .wav file." << std::endl;
        return -1;
    }

    sf::Sound paddle_sound(paddle_buffer);
    sf::Sound wall_sound(wall_buffer);

    sf::RectangleShape ai_paddle({20, 60});
    sf::RectangleShape player_paddle({20, 60});

    player_paddle.setOrigin({10, 30});
    player_paddle.setPosition({40, 300});
    player_paddle.setFillColor(sf::Color::White);

    ai_paddle.setOrigin({10, 30});
    ai_paddle.setPosition({760, 300});
    ai_paddle.setFillColor(sf::Color::White);

    circle.setFillColor(sf::Color::White);
    circle.setOrigin({10, 10});
    circle.setPosition({400, 300});

    sf::Vector2f d({10, 10});
    sf::Vector2f pd({d.x, d.y});
    sf::Vector2f player_d({d.x, d.y});

    while (window.isOpen()) {
        
        circle.move({d.x, d.y});
        ai_paddle.move({0, pd.y * 0.8f});

        if (circle.getPosition().y > 590) {
            d.y = -d.y;
            pd.y = -pd.y;
            wall_sound.play();
        } else if (circle.getPosition().y < 10) {
            d.y = -d.y;
            pd.y = -pd.y;
            wall_sound.play();
        } else if (circle.getPosition().x > 790) {
            d.x = -d.x;
            wall_sound.play();
        } else if (circle.getPosition().x < 10) {
            d.x = -d.x;
            wall_sound.play();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            player_paddle.move({0, -player_d.y});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            player_paddle.move({0, player_d.y});

        if (const std::optional intersection = ai_paddle.getGlobalBounds().findIntersection(circle.getGlobalBounds())) {
            d.x = -d.x;
            paddle_sound.play();
        }
        if (const std::optional intersection = player_paddle.getGlobalBounds().findIntersection(circle.getGlobalBounds())) {
            d.x = -d.x;
            paddle_sound.play();
        }

        while (const std::optional event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(circle);
        window.draw(player_paddle);
        window.draw(ai_paddle);
        window.display();
    }
}