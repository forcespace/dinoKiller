//
// Created by Сергей Павлов on 28.10.2021.
//

#ifndef PROJECT_ACTIVITIES_MAP_H
#define PROJECT_ACTIVITIES_MAP_H

#include <SFML/Graphics.hpp>

const int HEIGHT_MAP = 25;
const int WIDTH_MAP = 40;

sf::String TileMap[HEIGHT_MAP] = {
        "0000000000000000000000000000000000000000",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0                                      0",
        "0              s    0000               0",
        "0           000000000000               0",
        "0          0000000000000               0",
        "0   f    h00000000000000               0",
        "0000000000000000000000000000000000000000",
};

void randomMapGenerate()
{
    char const scoreMapSymbol = 's';
    char const healthDownMapSymbol = 'f';
    char const healthUpMapSymbol = 'h';

    int randomElementX = 0;
    int randomElementY = 0;
    srand(time(0));
    int countScoreItem = 40;
    int countHealthDownItem = 45;
    int countHealthUpItem = 20;

    while (countScoreItem > 0)
    {
        randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

        if (TileMap[randomElementY][randomElementX] == ' ')
        {
            TileMap[randomElementY][randomElementX] = scoreMapSymbol;
            countScoreItem--;
        }
    }

    while (countHealthDownItem > 0)
    {
        randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

        if (TileMap[randomElementY][randomElementX] == ' ')
        {
            TileMap[randomElementY][randomElementX] = healthDownMapSymbol;
            countHealthDownItem--;
        }
    }

    while (countHealthUpItem > 0)
    {
        randomElementX = 1 + rand() % (WIDTH_MAP - 1);
        randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

        if (TileMap[randomElementY][randomElementX] == ' ')
        {
            TileMap[randomElementY][randomElementX] = healthUpMapSymbol;
            countHealthUpItem--;
        }
    }
}
#endif //PROJECT_ACTIVITIES_MAP_H
