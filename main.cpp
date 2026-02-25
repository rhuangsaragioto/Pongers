#include "Pong.hpp"
#include "Powerup.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>

static const float WIDTH = 800.f;
static const float HEIGHT = 600.f;

static const float BALL_RADIUS = 10.f;
static const float BASE_SPEED = 6.f;

static const float PADDLE_WIDTH = 20.f;
static const float PADDLE_HEIGHT = 60.f;

static const float POWERUP_DUR = 5.f;
static const float POWERUP_SPAWN_INTERVAL = 8.f;

static const sf::Color WHITE = sf::Color::White;

static void resetBallPos(sf::CircleShape& ball, sf::Vector2f& d) {
    ball.setPosition({WIDTH / 2.f, HEIGHT / 2.f});

    float velocity_y = ((std::rand() % 2) == 0 ) ? BASE_SPEED : -BASE_SPEED;  // goes to random side after scoring point.
    d = {BASE_SPEED, velocity_y};
}

int main() {
    std::srand(static_cast<unsigned>(std::time(NULL)));

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(WIDTH), static_cast<unsigned int>(HEIGHT)}), "Pongers");
    window.setFramerateLimit(60);

    // fonts
    sf::Font font;
    if (!font.openFromFile("assets/fonts/OpenSans-Light.ttf")) {
        std::cerr << "Couldn't open .ttf file." << std::endl;
        return -1;
    }

    // sounds
    sf::SoundBuffer paddle_buffer, wall_buffer;
    if (!paddle_buffer.loadFromFile("assets/sounds/paddle_hit.wav") ||
        !wall_buffer.loadFromFile("assets/sounds/wall_hit.wav")) {
            std::cerr <<  "Couldn't open .wav file" << std::endl;
            return -1;
        }
    sf::Sound paddle_sound(paddle_buffer),
              wall_sound(wall_buffer);
    
    // ball
    sf::CircleShape ball(BALL_RADIUS);
    ball.setFillColor(WHITE);
    ball.setOrigin({BALL_RADIUS, BALL_RADIUS});
    sf::Vector2f direction;
    resetBallPos(ball, direction);

    // paddles
    sf::RectangleShape player_paddle({PADDLE_WIDTH, PADDLE_HEIGHT});
    player_paddle.setOrigin({PADDLE_WIDTH / 2.f, PADDLE_HEIGHT / 2.f});
    player_paddle.setPosition({40.f, PADDLE_HEIGHT / 2.f});
    player_paddle.setFillColor(WHITE);
    
    sf::RectangleShape ai_paddle({PADDLE_WIDTH, PADDLE_HEIGHT});
    ai_paddle.setOrigin({PADDLE_WIDTH / 2.f, PADDLE_HEIGHT / 2.f});
    ai_paddle.setPosition({WIDTH - 40.f, HEIGHT / 2.f});
    ai_paddle.setFillColor(WHITE);

    float paddle_speed = BASE_SPEED;
    float ball_speed = BASE_SPEED;

    // scores
    int player_score = 0;
    int ai_score = 0;
    sf::Text score_text(font, "0  :  0", 32);
    score_text.setFillColor(WHITE);
    score_text.setOrigin({score_text.getLocalBounds().size.x / 2.f, 0.f});
    score_text.setPosition({WIDTH / 2.f, 10.f});

    auto update_score = [&](){
        score_text.setString(std::to_string(player_score) + "  :  " + std::to_string(ai_score));
        score_text.setOrigin({score_text.getLocalBounds().size.x / 2.f, 0.f});
        score_text.setPosition({WIDTH / 2.f, 10.f});
    };

    // division
    sf::RectangleShape division({2.f, 10.f});
    division.setFillColor(sf::Color(255, 255, 255, 100));

    // powerups
    std::vector<Powerup> powerups;

    sf::Clock spawn_clock;
    sf::Clock effect_clock;
    
    bool active = false;
    PowerupType active_effect = PowerupType::SpeedBoost;
    float remaining_time = 0.f;

    // effect text
    sf::Text effect_text(font, "", 18);
    effect_text.setFillColor(sf::Color::Yellow);
    effect_text.setPosition({10.f, HEIGHT - 30.f});

    auto applyEffect = [&](PowerupType pt){
        if (active) {
            switch (active_effect) {
                case PowerupType::SpeedBoost:
                    direction = (direction / ball_speed) * BASE_SPEED;
                    ball_speed = BASE_SPEED;
                    break;
                case PowerupType::SpeedDecrease:
                    direction  = (direction / ball_speed) * BASE_SPEED;
                    ball_speed = BASE_SPEED;
                    break;
                case PowerupType::BigPaddle:
                    player_paddle.setSize({WIDTH, HEIGHT});
                    player_paddle.setOrigin({WIDTH / 2.f, HEIGHT / 2.f});
                    break;
                case PowerupType::SmallPaddle:
                    player_paddle.setSize({WIDTH, HEIGHT});
                    player_paddle.setOrigin({WIDTH / 2.f, HEIGHT / 2.f});
                    break;
            }
        }

        switch (pt) {
            case PowerupType::SpeedBoost:
                ball_speed = BASE_SPEED * 1.7f;
                direction = (direction / BASE_SPEED) * ball_speed;
                effect_text.setString("SPEED BOOST!");
                break;
            case PowerupType::SpeedDecrease:
                ball_speed = BASE_SPEED * 0.5f;
                direction = (direction / BASE_SPEED) * ball_speed;
                effect_text.setString("SLOW BALL");
                break;
            case PowerupType::BigPaddle:
                player_paddle.setSize({PADDLE_WIDTH, PADDLE_HEIGHT * 2.f});
                player_paddle.setOrigin({PADDLE_WIDTH / 2.f, PADDLE_HEIGHT});
                effect_text.setString("BIG PADDLE!");
                break;
            case PowerupType::SmallPaddle:
                player_paddle.setSize({PADDLE_WIDTH, PADDLE_HEIGHT * 0.5f});
                player_paddle.setOrigin({PADDLE_WIDTH / 2.f, PADDLE_HEIGHT * 0.25f});
                effect_text.setString("TINY PADDLE...");
                break;
        }

        active_effect = pt;
        remaining_time = POWERUP_DUR;
        active = true;
    };

    auto revertEffect = [&](){
        if (!active)
            return;
        switch (active_effect) {
            case PowerupType::SpeedBoost:
                direction = (direction / ball_speed) * BASE_SPEED;
                ball_speed = BASE_SPEED;
                break;
            case PowerupType::SpeedDecrease:
                direction = (direction / ball_speed) * BASE_SPEED;
                ball_speed = BASE_SPEED;
                break;
            case PowerupType::BigPaddle:
                player_paddle.setSize({PADDLE_WIDTH, PADDLE_HEIGHT});
                player_paddle.setOrigin({PADDLE_WIDTH/ 2.f, PADDLE_HEIGHT / 2.f});
                break;
            case PowerupType::SmallPaddle:
                player_paddle.setSize({PADDLE_WIDTH, PADDLE_HEIGHT});
                player_paddle.setOrigin({PADDLE_WIDTH / 2.f, PADDLE_HEIGHT / 2.f});
                break;
        }
        active = false;
        effect_text.setString("");
    };

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float half_height = player_paddle.getSize().y / 2.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            float newY = player_paddle.getPosition().y - paddle_speed;
            player_paddle.setPosition({player_paddle.getPosition().x,
                                       std::max(newY, half_height)});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            float newY = player_paddle.getPosition().y + paddle_speed;
            player_paddle.setPosition({player_paddle.getPosition().x,
                                       std::min(newY, HEIGHT - half_height)});
        }

        float ai_y = ai_paddle.getPosition().y;
        float ball_y = ball.getPosition().y;
        float ai_half_height = ai_paddle.getSize().y / 2.f;
        float ai_speed = BASE_SPEED * 0.75f;
        if (ball_y < (ai_y - 5.f))
            ai_paddle.setPosition({ai_paddle.getPosition().x,
                                   std::max(ai_y - ai_speed, ai_half_height)});
        else if (ball_y > (ai_y + 5.f))
            ai_paddle.setPosition({ai_paddle.getPosition().x,
                                   std::min(ai_y + ai_speed, HEIGHT - ai_half_height)});
        
        ball.move(direction);

        if (ball.getPosition().y - BALL_RADIUS < 0.f) {
            ball.setPosition({ball.getPosition().x, BALL_RADIUS});
            direction.y = std::abs(direction.y);
            wall_sound.play();
        } else if (ball.getPosition().y + BALL_RADIUS > HEIGHT) {
            ball.setPosition({ball.getPosition().x, HEIGHT - BALL_RADIUS});
            direction.y = -std::abs(direction.y);
            wall_sound.play();
        }

        if (ball.getPosition().x - BALL_RADIUS < 0.f) {
            ai_score += 1;
            update_score();
            revertEffect();
            powerups.clear();
            resetBallPos(ball, direction);
            ball_speed = BASE_SPEED;
        } else if (ball.getPosition().x + BALL_RADIUS > WIDTH) {
            player_score += 1;
            update_score();
            revertEffect();
            powerups.clear();
            resetBallPos(ball, direction);
            ball_speed = BASE_SPEED;
        }

        auto handleCollision = [&](sf::RectangleShape& paddle) {
            if (paddle.getGlobalBounds().findIntersection(ball.getGlobalBounds())) {
                direction.x = -direction.x;
                float angle = (ball.getPosition().y - paddle.getPosition().y)
                              / (paddle.getSize().y / 2.f);
                direction.y = angle * ball_speed * 0.8f;
                paddle_sound.play();
            }
        };
        handleCollision(player_paddle);
        handleCollision(ai_paddle);

        if (spawn_clock.getElapsedTime().asSeconds() > POWERUP_SPAWN_INTERVAL) {
            spawn_clock.restart();

            float px = WIDTH / 2.f + (std::rand() % 200 - 100);
            float py = 50.f + std::rand() % static_cast<int>(HEIGHT - 100.f);
            powerups.emplace_back(Powerup::genRandomType(), sf::Vector2f({px, py}), font);
        }

        for (auto& powerup : powerups) {
            if (powerup.isActive() && powerup.getBounds().findIntersection(ball.getGlobalBounds())) {
                applyEffect(powerup.getType());
                powerup.collectPowerup();
            }
        }

        powerups.erase(
            std::remove_if(powerups.begin(), powerups.end(),
                [](const Powerup& pu){ return !pu.isActive(); }),
            powerups.end()
        );

        if (active) {
            remaining_time -= dt;
            if (remaining_time <= 0.f)
                revertEffect();
        }
        
        window.clear(sf::Color(20, 20, 30));

        for (int i = 0; i < 30; i++) {
            division.setPosition({(WIDTH / 2.f) - 1.f, 10.f + (i * 20.f)});
            window.draw(division);
        }

        window.draw(ball);
        window.draw(player_paddle);
        window.draw(ai_paddle);
        window.draw(score_text);
        window.draw(effect_text);
        for (auto& powerup : powerups)
            window.draw(powerup);
        window.display();
    }

    return 0;
}