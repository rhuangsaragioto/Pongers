// Copyright 2026 <Rhuan Saragioto>

#pragma once

#ifndef PONG_HPP
#define PONG_HPP

#include <SFML/Graphics.hpp>

class Pong : public sf::Drawable {
 private:
    sf::Vector2f _paddleSize;
    sf::Vector2f _paddlePosition;

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
    Pong();
    ~Pong() = default;

    void handleCollision(const sf::Event& event);
    void update(float dt);
};

#endif