#include "Pong.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, const char* argv[]) {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Pongers");
    window.setFramerateLimit(60);

    sf::CircleShape circle(10);

    sf::RectangleShape ai_paddle({20, 60});

    ai_paddle.setOrigin({10, 30});
    ai_paddle.setPosition({760, 300});
    ai_paddle.setFillColor(sf::Color::White);

    circle.setFillColor(sf::Color::White);
    circle.setOrigin({10, 10});
    circle.setPosition({400, 300});

    sf::Vector2f d({5, 5});
    sf::Vector2f pd({d.x, d.y});

    while (window.isOpen()) {
        
        circle.move({d.x, d.y});
        ai_paddle.move({0, pd.y * 0.9f});

        if (circle.getPosition().y > 590) {
            d.y = -d.y;
            pd.y = -pd.y;
        } else if (circle.getPosition().y < 10) {
            d.y = -d.y;
            pd.y = -pd.y;
        } else if (circle.getPosition().x > 790) {
            d.x = -d.x;
        } else if (circle.getPosition().x < 10) {
            d.x = -d.x;
        }

        if (const std::optional intersection = ai_paddle.getGlobalBounds().findIntersection(circle.getGlobalBounds())) {
            d.x = -d.x;
        }

        while (const std::optional event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(circle);
        window.draw(ai_paddle);
        window.display();
    }
}