//#include <iostream>
//#include <SFML/Graphics.hpp>
//#include <filesystem>
//#include "src/Player.h"
//
//constexpr unsigned WINDOW_WIDTH = 1920;
//constexpr unsigned WINDOW_HEIGHT = 1080;
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Application");
//
//    Player dino("hero.png", 150, 250, 88, 94);
//
//    double currentFrame = 0;
//    sf::Clock clock;
//
//    while (window.isOpen())
//    {
//        float time = clock.getElapsedTime().asMicroseconds();
//        clock.restart();
//        time = time / 800;
//
//        sf::Event event{};
//
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//        {
//            dino.dir = 3;
//            dino.speed = 0.1;
//            currentFrame += 0.011 * time;
//            if (currentFrame > 4)
//            {
//                currentFrame -= 2;
//            }
//            dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
//        }
//        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//        {
//            dino.dir = 0;
//            dino.speed = 0.1;
//            currentFrame += 0.011 * time;
//            if (currentFrame > 4)
//            {
//                currentFrame -= 2;
//            }
//            dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
//        }
//        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//        {
//            dino.dir = 2;
//            dino.speed = 0.1;
//            currentFrame += 0.011 * time;
//            if (currentFrame > 4)
//            {
//                currentFrame -= 2;
//            }
//            dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
//        }
//        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//        {
//            dino.dir = 1;
//            dino.speed = 0.1;
//            currentFrame += 0.011 * time;
//            if (currentFrame > 4)
//            {
//                currentFrame -= 2;
//            }
//            dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
//        }
//
//        dino.update(time);
//
//        window.clear();
//        window.draw(dino.sprite);
//        window.display();
//    }
//}