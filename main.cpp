#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "src/map.h"
#include "src/view.h"
#include "src/level.h"
#include "src/LifeBar.h"
#include <vector>
#include <list>

constexpr unsigned WINDOW_WIDTH = 1920;
constexpr unsigned WINDOW_HEIGHT = 1080;
constexpr unsigned CAM_WIDTH = 1200;
constexpr unsigned CAM_HEIGHT = 600;

class Entity
{
public:
    std::vector<Object> obj;
    float dx, dy, x, y, speed;
    int w, h, health;
    bool life, onGround;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::String name;

    Entity(sf::Image &image, const sf::String &Name, float X, float Y, int W, int H)
    {
        x = X;
        y = Y;
        w = W;
        h = H;
        name = Name;
        speed = 0;
        health = 100;
        dx = 0;
        dy = 0;
        life = true;
        onGround = false;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w / 2, h / 2);
    }

    sf::FloatRect getRect()
    {
        return sf::FloatRect(x, y, w, h);
    }

    virtual void update(float time) = 0;
};

class Player : public Entity
{
public:
    enum
    {
        left, right, up, down, jump, stay
    } state;

    int playerScore;

    Player(sf::Image &image, const sf::String &Name, Level &lev, float X, float Y, int W, int H) : Entity(image, Name, X, Y, W, H)
    {
        playerScore = 0;
        state = stay;
        obj = lev.GetAllObjects();

        if (name == "Player1")
        {
            sprite.setTextureRect(sf::IntRect(0, 0, w, h));
        }
    }

    void control()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            state = left;
            speed = 0.1;
//                currentFrame += 0.011 * time;
//                if (currentFrame > 4)
//                {
//                    currentFrame -= 2;
//                }
//                dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            state = right;
            speed = 0.1;
//                currentFrame += 0.011 * time;
//                if (currentFrame > 4)
//                {
//                    currentFrame -= 2;
//                }
//                dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && (onGround))
        {
            state = jump;
            dy = -.65;
            onGround = false;//то состояние равно прыжок,прыгнули и сообщили, что мы не на земле
////                currentFrame += 0.011 * time;
////                if (currentFrame > 4)
////                {
////                    currentFrame -= 2;
////                }
////                dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            state = down;
            speed = 0.1;
////                currentFrame += 0.011 * time;
////                if (currentFrame > 4)
////                {
////                    currentFrame -= 2;
////                }
////                dino.sprite.setTextureRect(sf::IntRect(88 * int(currentFrame), 0, 88, 94));
        }
    }

    void checkCollisionWithMap(float Dx, float Dy)
    {
        for (int i = 0; i < obj.size(); i++)
        {
            if (getRect().intersects(obj[i].rect))
            {
                if (obj[i].name == "solid")
                {
                    if (Dy > 0)
                    {
                        y = obj[i].rect.top - h;
                        dy = 0;
                        onGround = true;
                    }
                    if (Dy < 0)
                    {
                        y = obj[i].rect.top + obj[i].rect.height;
                        dy = 0;
                    }
                    if (Dx > 0)
                    { x = obj[i].rect.left - w; }
                    if (Dx < 0)
                    { x = obj[i].rect.left + obj[i].rect.width; }
                }
            }
        }
    }

    void update(float time)
    {
        control();

        switch (state)
        {
            case right:
                dx = speed;
                break;
            case left:
                dx = -speed;
                break;
            case up:
                break;
            case down:
                break;
            case jump:
                break;
            case stay:
                break;
        }

        speed = 0;

        x += dx * time;
        checkCollisionWithMap(dx, 0);

        y += dy * time;
        checkCollisionWithMap(0, dy);

        sprite.setPosition(x + w / 2, y + h / 2);

        if (health <= 0)
        {
            life = false;
        }

        if (life)
        {
            getPlayerCoordinateForView(x, y);
        }

        dy = dy + 0.0015 * time;
    }
};

class Enemy : public Entity
{
public:
    Enemy(sf::Image &image, const sf::String &Name, Level &lvl, float X, float Y, int W, int H) : Entity(image, Name, X, Y, W, H)
    {
        obj = lvl.GetObjects("solid");

        if (name == "easyEnemy")
        {
            sprite.setTextureRect(sf::IntRect(0, 0, w, h));
            dx = 0.1;
        }
    }

    void checkCollisionWithMap(float Dx, float Dy)
    {
        for (int i = 0; i < obj.size(); i++)
        {
            if (getRect().intersects(obj[i].rect))
            {
                if (Dy > 0)
                {
                    y = obj[i].rect.top - h;
                    dy = 0;
                    onGround = true;
                }
                if (Dy < 0)
                {
                    y = obj[i].rect.top + obj[i].rect.height;
                    dy = 0;
                }
                if (Dx > 0)
                {
                    x = obj[i].rect.left - w;
                    dx = -0.1;
                    sprite.scale(-1, 1);
                }
                if (Dx < 0)
                {
                    x = obj[i].rect.left + obj[i].rect.width;
                    dx = 0.1;
                    sprite.scale(-1, 1);
                }
            }
        }
    }

    void update(float time)
    {
        if (name == "easyEnemy")
        {
            checkCollisionWithMap(dx, dy);
            x += dx * time;
            sprite.setPosition(x + w / 2, y + h / 2);

            if (health <= 0)
            {
                life = false;
            }
        }
    }
};

int main()
{
    randomMapGenerate();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Application");
    view.reset(sf::FloatRect(0, 0, CAM_WIDTH, CAM_HEIGHT));

    Level lvl;
    lvl.LoadFromFile("src/map3.tmx");

    sf::Music music;//создаем объект музыки
    music.openFromFile("music.ogg");//загружаем файл
    music.play();//воспроизводим музыку

    sf::Font font;
    font.loadFromFile("upload/font/EuclidCircularB-Regular.ttf");
    sf::Text healthText("", font, 20);
    sf::Text timeText("", font, 20);
    sf::Text scoreText("", font, 20);

    healthText.setColor(sf::Color::Red);
    timeText.setColor(sf::Color::Red);
    scoreText.setColor(sf::Color::Red);

    sf::Image heroImage;
    heroImage.loadFromFile("upload/images/hero2.png");

    sf::Image easyEnemyImage;
    easyEnemyImage.loadFromFile("upload/images/cactus2.png");

    std::list<Entity *> entities;
    std::list<Entity *>::iterator it;
    std::list<Entity *>::iterator it2;

    std::vector<Object> e = lvl.GetObjects("easyEnemy");

    for (int i = 0; i < e.size(); i++)
    {
        entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 28, 37));
    }

    Object player = lvl.GetObject("player");

    Player dino(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 44, 47);

    sf::Clock clock;
    sf::Clock gameTimeClock;
    int gameTime = 0;

    LifeBar lifeBarPlayer;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();

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

        lifeBarPlayer.update(100);


        dino.update(time);

        for (it = entities.begin(); it != entities.end(); it++)
        {
            (*it)->update(time);
        }

        for (it = entities.begin(); it != entities.end(); it++)
        {
            if ((*it)->getRect().intersects(dino.getRect()))
            {
                if ((*it)->name == "easyEnemy")
                {
                    if ((*it)->dx > 0)
                    {
                        (*it)->x = dino.x - (*it)->w;
                        (*it)->dx = 0;
                    }

                    if ((*it)->dx < 0)
                    {
                        (*it)->x = dino.x + dino.w;
                        (*it)->dx = 0;
                    }
                }
            }

            for (it2 = entities.begin(); it2 != entities.end(); it2++)
            {
                if ((*it)->getRect() != (*it2)->getRect())
                {
                    if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "easyEnemy") && ((*it2)->name == "easyEnemy"))
                    {
                        (*it)->dx *= -1;
                        (*it)->sprite.scale(-1, 1);
                    }
                }
            }
        }

        window.setView(view);
        window.clear();
        lvl.Draw(window);

        for (it = entities.begin(); it != entities.end(); it++)
        {
            window.draw((*it)->sprite);
        }

        lifeBarPlayer.draw(window);
        window.draw(dino.sprite);
        window.display();
    }

    return 0;
}