#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "src/map.h"
#include "src/view.h"

constexpr unsigned WINDOW_WIDTH = 1920;
constexpr unsigned WINDOW_HEIGHT = 1080;
constexpr unsigned CAM_WIDTH = 1200;
constexpr unsigned CAM_HEIGHT = 674;

class Player
{
private:
    float x, y = 0;
public:
    float w, h, dx{}, dy{}, speed = 0;
    int dir, playerScore, health = 0;
    bool life;
    sf::String File;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    Player(const sf::String &F, int X, int Y, float W, float H)
    {
        dir = 0;
        playerScore = 0;
        health = 100;
        life = true;
        File = F;
        w = W;
        h = H;
        image.loadFromFile("upload/images/" + File);
        image.createMaskFromColor(sf::Color(41, 33, 59));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X;
        y = Y;
        sprite.setTextureRect(sf::IntRect(0, 0, w, h));
    }

    void update(float time)
    {
        switch (dir)
        {
            case 0:
                dx = speed;
                dy = 0;
                break;
            case 1:
                dx = -speed;
                dy = 0;
                break;
            case 2:
                dx = 0;
                dy = speed;
                break;
            case 3:
                dx = 0;
                dy = -speed;
                break;
        }

        x += dx * time;
        y += dy * time;

        speed = 0;
        sprite.setPosition(x, y);
        interactionWithMap();

        if (health <= 0)
        {
            life = false;
        }
    }

    float getPlayerCoordinateX()
    {
        return x;
    }

    float getPlayerCoordinateY()
    {
        return y;
    }

    void interactionWithMap()
    {
        for (int i = y / 32; i < (y + h) / 32; i++)
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')
                {
                    if (dy > 0)
                    {
                        y = i * 32 - h;
                    }
                    if (dy < 0)
                    {
                        y = i * 32 + 32;
                    }
                    if (dx > 0)
                    {
                        x = j * 32 - w;
                    }
                    if (dx < 0)
                    {
                        x = j * 32 + 32;
                    }
                }

                if (TileMap[i][j] == 's')
                {
                    playerScore++;
                    TileMap[i][j] = ' ';
                }

                if (TileMap[i][j] == 'f')
                {
                    health -= 50;
                    TileMap[i][j] = ' ';
                }

                if (TileMap[i][j] == 'h')
                {
                    health += 10;
                    TileMap[i][j] = ' ';
                }
            }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Application");
    view.reset(sf::FloatRect(0, 0, CAM_WIDTH, CAM_HEIGHT));

    sf::Font font;
    font.loadFromFile("upload/font/EuclidCircularB-Regular.ttf");
    sf::Text healthText("", font, 20);
    sf::Text timeText("", font, 20);
    healthText.setColor(sf::Color::Red);
    timeText.setColor(sf::Color::Red);

    sf::Image map_image;
    map_image.loadFromFile("upload/images/map.png");
    sf::Texture map;
    map.loadFromImage(map_image);
    sf::Sprite s_map;
    s_map.setTexture(map);

    Player dino("hero.png", 150, 250, 88, 94);

    double currentFrame = 0;
    sf::Clock clock;
    sf::Clock gameTimeClock;
    int gameTime = 0;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();

        if (dino.life)
        {
            gameTime = gameTimeClock.getElapsedTime().asSeconds();
        }

        clock.restart();
        time = time / 800;

        sf::Event event{};

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (dino.life)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                dino.dir = 3;
                dino.speed = 0.1;
                currentFrame += 0.011 * time;
                if (currentFrame > 4)
                {
                    currentFrame -= 2;
                }
                dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                dino.dir = 0;
                dino.speed = 0.1;
                currentFrame += 0.011 * time;
                if (currentFrame > 4)
                {
                    currentFrame -= 2;
                }
                dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                dino.dir = 2;
                dino.speed = 0.1;
                currentFrame += 0.011 * time;
                if (currentFrame > 4)
                {
                    currentFrame -= 2;
                }
                dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                dino.dir = 1;
                dino.speed = 0.1;
                currentFrame += 0.011 * time;
                if (currentFrame > 4)
                {
                    currentFrame -= 2;
                }
                dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
            }
        }

        getPlayerCoordinateForView(dino.getPlayerCoordinateX(), dino.getPlayerCoordinateY());
        dino.update(time);
        window.setView(view);
        window.clear();

        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ')
                {
                    s_map.setTextureRect(sf::IntRect(0, 0, 32, 32));
                }
                if (TileMap[i][j] == 's')
                {
                    s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));
                }
                if (TileMap[i][j] == '0')
                {
                    s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));
                }
                if (TileMap[i][j] == 'f')
                {
                    s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));
                }
                if (TileMap[i][j] == 'h')
                {
                    s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));
                }

                s_map.setPosition(j * 32, i * 32);

                window.draw(s_map);

                std::ostringstream playerHealthString, gameTimeString;
                playerHealthString << dino.health;
                gameTimeString << gameTime;
                healthText.setString("Health: " + playerHealthString.str());
                healthText.setPosition(view.getCenter().x + (float) CAM_WIDTH / 2 - 125, view.getCenter().y - (float) CAM_HEIGHT / 2 + 20);
                timeText.setString("Time: " + gameTimeString.str());
                timeText.setPosition(view.getCenter().x + (float) CAM_WIDTH - CAM_WIDTH + 125, view.getCenter().y - (float) CAM_HEIGHT / 2 + 20);
            }

        window.draw(dino.sprite);
        window.draw(healthText);
        window.draw(timeText);
        window.display();
    }
}