#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "src/view.h"
#include "src/level.h"
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

    float currentFrame;

    Player(sf::Image &image, const sf::String &Name, Level &lev, float X, float Y, int W, int H) : Entity(image, Name, X, Y, W, H)
    {
        state = stay;
        obj = lev.GetAllObjects();
        currentFrame = 0;

        if (name == "dino")
        {
            sprite.setTextureRect(sf::IntRect(0, 0, w, h));
        }
    }

    void checkCollisionWithMap(float Dx, float Dy)
    {
        for (auto & i : obj)
        {
            if (getRect().intersects(i.rect))
            {
                if (i.name == "solid")
                {
                    if (Dy > 0)
                    {
                        y = i.rect.top - h;
                        dy = 0;
                        onGround = true;
                    }
                    if (Dy < 0)
                    {
                        y = i.rect.top + i.rect.height;
                        dy = 0;
                    }
                    if (Dx > 0)
                    { x = i.rect.left - w; }
                    if (Dx < 0)
                    { x = i.rect.left + i.rect.width; }
                }

                if (i.name == "end")
                {
                    if (Dy > 0)
                    {
                        y = i.rect.top - h;
                        dy = 0;
                        onGround = true;
                    }
                    if (Dy < 0)
                    {
                        y = i.rect.top + i.rect.height;
                        dy = 0;
                    }
                    if (Dx > 0)
                    { x = i.rect.left - w; }
                    if (Dx < 0)
                    { x = i.rect.left + i.rect.width; }
                }
            }
        }
    }

    void update(float time) override
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            state = left;
            speed = 0.1;
            currentFrame += 0.011 * time;
            if (currentFrame > 4)
            {
                currentFrame -= 2;
            }
            sprite.setTextureRect(sf::IntRect(44 * int(currentFrame), 0, 44, 47));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            state = right;
            speed = 0.1;
            currentFrame += 0.011 * time;
            if (currentFrame > 4)
            {
                currentFrame -= 2;
            }
            sprite.setTextureRect(sf::IntRect(44 * int(currentFrame), 0, 44, 47));
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && (onGround))
        {
            state = jump;
            dy = -.65;
            onGround = false;
            currentFrame += 0.011 * time;
            if (currentFrame > 4)
            {
                currentFrame -= 2;
            }
            sprite.setTextureRect(sf::IntRect(44 * int(currentFrame), 0, 44, 47));
        }

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
        for (auto & i : obj)
        {
            if (getRect().intersects(i.rect))
            {
                if (Dy > 0)
                {
                    y = i.rect.top - h;
                    dy = 0;
                    onGround = true;
                }
                if (Dy < 0)
                {
                    y = i.rect.top + i.rect.height;
                    dy = 0;
                }
                if (Dx > 0)
                {
                    x = i.rect.left - w;
                    dx = -0.1;
                    sprite.scale(-1, 1);
                }
                if (Dx < 0)
                {
                    x = i.rect.left + i.rect.width;
                    dx = 0.1;
                    sprite.scale(-1, 1);
                }
            }
        }
    }

    void update(float time) override
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

void changeLevel(Level &lvl, int &numberLevel)
{
    if (numberLevel == 1)
    {
        lvl.LoadFromFile("src/map1.tmx");
    }

    else if (numberLevel == 2)
    {
        lvl.LoadFromFile("src/map2.tmx");
    }

    else
    {
        lvl.LoadFromFile("src/map1.tmx");
    }
}

bool startGame(sf::RenderWindow &window, int &numberLevel)
{
    view.reset(sf::FloatRect(0, 0, CAM_WIDTH, CAM_HEIGHT));

    Level lvl;
    changeLevel(lvl, numberLevel);

    sf::Music music;
    music.openFromFile("upload/sound/music.ogg");
    music.play();

    sf::Image heroImage;
    heroImage.loadFromFile("upload/images/hero2.png");

    sf::Image easyEnemyImage;
    easyEnemyImage.loadFromFile("upload/images/cactus2.png");

    std::list<Entity *> entities;
    std::list<Entity *>::iterator it;
    std::list<Entity *>::iterator it2;

    std::vector<Object> e = lvl.GetObjects("easyEnemy");

    for (auto & i : e)
    {
        entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, i.rect.left, i.rect.top, 28, 37));
    }

    Object player = lvl.GetObject("player");

    Player dino(heroImage, "dino", lvl, player.rect.left, player.rect.top, 44, 47);

    sf::Clock clock;
    sf::Clock gameTimeClock;

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        {
            lvl.levelNumber++;
            return true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
        {
            numberLevel = 0;
            return true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            return false;
        }

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
                        numberLevel = 0;
                        return true;
                    }

                    if ((*it)->dx < 0)
                    {
                        (*it)->x = dino.x + dino.w;
                        (*it)->dx = 0;
                        numberLevel = 0;
                        return true;
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

        window.draw(dino.sprite);
        window.display();
    }
}

void gameRunning(sf::RenderWindow &window, int &numberLevel)
{
    if (startGame(window, numberLevel))
    {
        numberLevel++;
        gameRunning(window, numberLevel);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Application");
    int numberLevel = 1;

    gameRunning(window, numberLevel);

    return 0;
}