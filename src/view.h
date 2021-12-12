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

//    if (x < 57)
//    {
//        tempX = 57;
//    }
//    else if (y < 57)
//    {
//        tempY = 57;
//    }
//    else if (y > 57)
//    {
//        tempY = 57;
//    }

    view.setCenter(tempX, tempY);
}

#endif //PROJECT_ACTIVITIES_VIEW_H