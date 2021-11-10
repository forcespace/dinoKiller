//
// Created by Сергей Павлов on 07.11.2021.
//

#ifndef PROJECT_ACTIVITIES_VIEW_H
#define PROJECT_ACTIVITIES_VIEW_H

#include <SFML/Graphics.hpp>

constexpr unsigned CAM_DEFAULT_WIDTH = 1200;
constexpr unsigned CAM_DEFAULT_HEIGHT = 678;
constexpr unsigned CAM_NEW_WIDTH = 1920;
constexpr unsigned CAM_NEW_HEIGHT = 1080;

sf::View view;

void getPlayerCoordinateForView(float x, float y)
{
    float tempX = x;
    float tempY = y;

    if (x < 200)
    {
        tempX = 200;
    }
    else if (y < 240)
    {
        tempY = 240;
    }
    else if (y > 554)
    {
        tempY = 554;
    }

    view.setCenter(tempX, tempY);
}

sf::View viewMap(float time)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        view.move(0, -0.1 * time);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        view.move(0.1 * time, 0);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        view.move(0, 0.1 * time);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        view.move(-0.1 * time, 0);
    }

    return view;
}

sf::View changeView()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
        view.setSize(CAM_DEFAULT_WIDTH, CAM_DEFAULT_HEIGHT);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        view.setSize(CAM_NEW_WIDTH, CAM_NEW_HEIGHT);
    }
}

#endif //PROJECT_ACTIVITIES_VIEW_H