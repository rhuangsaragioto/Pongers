// Copyright 2026 <Rhuan Saragioto>

#pragma once

#ifndef POWERUP_HPP
#define POWERUP_HPP

#include <SFML/Graphics.hpp>
#include <string>

enum class PowerupType {
     SpeedBoost,
     SpeedDecrease,
     BigPaddle,
     SmallPaddle,
};

class Powerup : public sf::Drawable {
 private:
     sf::CircleShape _shape;
     sf::Text _label;
     PowerupType _type;
     bool _active;

     static sf::Color typeColor(PowerupType pt);
     static std::string typeLabel(PowerupType pt);

 protected:
     void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
     Powerup(PowerupType pt, sf::Vector2f position, const sf::Font& font);
     ~Powerup() = default;

     sf::FloatRect getBounds() const;

     PowerupType getType() const { return _type; }
     bool isActive() const { return _active; }
     void collectPowerup() { _active = false; }

     static PowerupType genRandomType();
};

#endif