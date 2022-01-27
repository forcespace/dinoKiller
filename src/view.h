//
// Created by Сергей Павлов on 07.11.2021.
//

#ifndef PROJECT_ACTIVITIES_VIEW_H
#define PROJECT_ACTIVITIES_VIEW_H

#include <SFML/Graphics.hpp>

sf::View view;

sf::View getPlayerCoordinateForView(float x, float y)
{
    float tempX = x;
    float tempY = y;

    view.setCenter(tempX, tempY);

    return view;
}

#endif //PROJECT_ACTIVITIES_VIEW_H