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

sf::View getPlayerCoordinateForView(float x, float y)
{
    float tempX = x;
    float tempY = y;

    if (x < 100)
    {
        tempX = 100;
    }
    else if (y < 100)
    {
        tempY = 100;
    }

    view.setCenter(tempX, tempY);

    return view;
}

#endif //PROJECT_ACTIVITIES_VIEW_H