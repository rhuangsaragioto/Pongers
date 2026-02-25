// Copyright 2026 <Rhuan Saragioto>

#include "Powerup.hpp"

static const sf::Color SPEED_BOOST_COLOR = sf::Color(255, 100, 50);
static const sf::Color SPEED_DECREASE_COLOR = sf::Color(50, 150, 255);
static const sf::Color SMALL_PADDLE_COLOR = sf::Color(50, 220, 100);
static const sf::Color BIG_PADDLE_COLOR = sf::Color(220, 50, 220);

sf::Color Powerup::typeColor(PowerupType pt) {
    switch (pt) {
        case PowerupType::SpeedBoost:
            return SPEED_BOOST_COLOR;
        case PowerupType::SpeedDecrease:
            return SPEED_DECREASE_COLOR;
        case PowerupType::SmallPaddle:
            return SMALL_PADDLE_COLOR;
        case PowerupType::BigPaddle:
            return BIG_PADDLE_COLOR;
        default:
            return sf::Color::White;
    }
}

std::string Powerup::typeLabel(PowerupType pt) {
    switch (pt) {
        case PowerupType::SpeedBoost:
            return "S+";
        case PowerupType::SpeedDecrease:
            return "S-";
        case PowerupType::SmallPaddle:
            return "P+";
        case PowerupType::BigPaddle:
            return "P-";
        default:
            return "?";
    }
}

Powerup::Powerup(PowerupType pt, sf::Vector2f position, const sf::Font& font)
    : _label(font, typeLabel(pt), 12), _type(pt), _active(true) {

    _shape.setRadius(15.f);
    _shape.setOrigin({15.f, 15.f});
    _shape.setPosition(position);
    _shape.setFillColor(typeColor(pt));

    _label.setOrigin({_label.getLocalBounds().size / 2.f});
    _label.setPosition(position);
    _label.setFillColor(sf::Color::White);
}

void Powerup::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (_active) {
        target.draw(_shape, states);
        target.draw(_label, states);
    }
}

sf::FloatRect Powerup::getBounds() const {
    return _shape.getGlobalBounds();
}

PowerupType Powerup::genRandomType() {
    int r = std::rand() % 4;
    return static_cast<PowerupType>(r);
}