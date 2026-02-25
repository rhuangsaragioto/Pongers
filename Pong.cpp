// Copyright 2026 <Rhuan Saragioto>

#include "Pong.hpp"

Pong::Pong()
    : _paddleSize({20.f, 60.f}), _paddlePosition({40.f, 300.f}) {}

void Pong::draw(sf::RenderTarget& target, sf::RenderStates states) const {}

void Pong::handleCollision(const sf::Event& event) {}

void Pong::update(float dt) {}
