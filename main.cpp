#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <ctime>

#include <base.hpp>

const sf::Font FONT("arial.ttf");
bool DisplayVelocity = true;
bool DisplayInfo = true;

void Tick(float Time) {
    //Time is poorly handled but meh
    Time = Time * (10 * TimeSetting);
    for (auto& asteroid : asteroidlist) {
        asteroid.position.x += asteroid.velocity.x * Time;
        asteroid.position.y += asteroid.velocity.y * Time;

        for (auto& otherasteroid : asteroidlist) {
            if (otherasteroid.id == asteroid.id) {
                continue;
            }
            float distanceX = (otherasteroid.position.x - asteroid.position.x);
            float distanceY = (otherasteroid.position.y - asteroid.position.y);

            float distance = sqrt(distanceX*distanceX + distanceY*distanceY);

            float dx = distanceX / distance;
            float dy = distanceY / distance;

            float acceleration = (1 / (distance * distance + (otherasteroid.mass * 5)) * otherasteroid.mass);

            asteroid.velocity.x += dx * acceleration * Time;
            asteroid.velocity.y += dy * acceleration * Time;
        }
    }
}

int main()
{
    std::srand(std::time(nullptr));

    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Bluemoon");
    window.setPosition({0, 0});

    sf::Clock clock;
    float accumulator = 0.f;
    const float TICK_RATE = 0.01f;

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                const auto* mouseEvent =
                    event->getIf<sf::Event::MouseButtonPressed>();

                if (mouseEvent &&
                    mouseEvent->button == sf::Mouse::Button::Left)
                {
                    CreateRoid("__", mouseEvent->position, {1.f, 0.f}, 50, {2, 0});
                } else if (mouseEvent &&
                    mouseEvent->button == sf::Mouse::Button::Right)
                {
                    //lateeer
                    //for (int i=1; i < 5; i++) {
                    //    CreateRoid("asv1", mouseEvent->position + sf::Vector2i(rand()%20, rand()%20), {1.f, 0.f}, 50, {2, 0});
                    //}
                    CreateRoid("__", mouseEvent->position, {1.f, 0.f}, 500, {10, 0});
                }
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    window.close();
                }
                else
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::W) 
                    {
                        Asteroid earth;
                        earth = CreateRoid("earth", {400, 300}, {0.f, 0.f}, 1000.f, {8, 0});
                        Asteroid moon;
                        moon = CreateRoid("moon", {550, 300}, {0.f, static_cast<float>(sqrt(DAMPING * earth.mass / 150.f))}, 10.f, {3, 0});
                        break;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace) 
                    {
                        asteroidlist.clear();
                        break;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::I) 
                    {
                        DisplayInfo = !DisplayInfo;
                        break;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::V) 
                    {
                        DisplayVelocity = !DisplayVelocity;
                        break;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::RBracket) 
                    {
                        TimeSetting += 1;
                        break;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::LBracket) 
                    {
                        TimeSetting -= 1;
                        if (TimeSetting < 1) {TimeSetting = 1;}
                        break;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        float dt = clock.restart().asSeconds();
        accumulator += dt;

        while (accumulator >= TICK_RATE)
        {
            Tick(TICK_RATE);
            accumulator -= TICK_RATE;
        }

        for (auto& a : asteroidlist) {
            sf::CircleShape asteroidshape(a.size.x);
            asteroidshape.setPosition(sf::Vector2f(a.position.x, a.position.y));
            asteroidshape.setOrigin({static_cast<float>(a.size.x), static_cast<float>(a.size.x)});
            asteroidshape.setFillColor({255, 255, 255});

            sf::RectangleShape line({200.f, 0.5f});
            line.setPosition(asteroidshape.getPosition());
            line.setFillColor({255, 0, 255});
            line.rotate(sf::degrees(-45));

            sf::Text text(FONT);
            text.setString(a.name);
            text.setFillColor({255, 0, 255});
            text.setPosition(line.getPosition() + sf::Vector2f({150, -150}));

            std::array line2 =
            {
                sf::Vertex{sf::Vector2f(
                    asteroidshape.getPosition().x,
                    asteroidshape.getPosition().y
                )},
                sf::Vertex{sf::Vector2f(asteroidshape.getPosition().x + a.velocity.x * 100, asteroidshape.getPosition().y + a.velocity.y * 100)}
            };

            if (DisplayVelocity) {window.draw(line2.data(), line2.size(), sf::PrimitiveType::Lines);};
            if (DisplayInfo) {window.draw(line); window.draw(text);};

            window.draw(asteroidshape);
        }

        std::string message1 = "DisplayVelocity is " + 
            std::string(DisplayVelocity ? "true" : "false");
        std::string message2 = "DisplayInfo is " + 
            std::string(DisplayInfo ? "true" : "false");

        sf::Text InformationText(FONT);
        InformationText.setString(
            "Time Multiplier: " + std::to_string(TimeSetting) + ", [ = -, ] = +"
            + "\n" + message1 + ", V to toggle"
            + "\n" + message2 + ", I to toggle"
            + "\n" + "W to spawn Earth & Moon"
        );
        InformationText.setFillColor({255, 0, 255});
        InformationText.setPosition({0, 0});
        window.draw(InformationText);

        window.display();
    }
}