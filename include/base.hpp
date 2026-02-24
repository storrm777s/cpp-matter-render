#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

int TimeSetting = 5;
int idl = 9;

const float DAMPING = 1;

struct vector2 {
    float x; float y;
};

struct Asteroid {
    std::string name;
    int id;
    int mass = 200;
    sf::Vector2u size = {2, 0};
    sf::Vector2f position = {200.f,200.f};
    sf::Vector2f velocity = {1.f,0.f};
    bool deletion=false;
};

std::vector<Asteroid> asteroidlist;

Asteroid CreateRoid(std::string name, sf::Vector2i pos, sf::Vector2f vel, float mass, sf::Vector2i size) {
    idl++;
    Asteroid asteroid;
    asteroid.id = idl;
    asteroid.mass = mass;
    asteroid.size = {static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y)};
    asteroid.position.x = pos.x;
    asteroid.position.y = pos.y;
    asteroid.velocity.x = vel.x;
    asteroid.velocity.y = vel.y;
    asteroid.name = name + " " + std::to_string(idl) + " | " + std::to_string(mass);
    asteroidlist.push_back(asteroid);
    return asteroid;
}