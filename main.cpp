#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "src/map.h"
#include "src/view.h"

constexpr unsigned WINDOW_WIDTH = 1920;
constexpr unsigned WINDOW_HEIGHT = 1080;
constexpr unsigned CAM_WIDTH = 1200;
constexpr unsigned CAM_HEIGHT = 674;

class Entity
{
public:
    float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей

    int w, h, health;

    bool life, onGround;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени

    Entity(sf::Image &image, float X, float Y, int W, int H, const sf::String &Name)
    {
        x = X;
        y = Y;
        w = W;
        h = H;
        name = Name;
        moveTimer = 0;
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
};

class Player : public Entity
{

public:

    enum stateObject
    {
        left,
        right,
        up,
        down,
        jump,
        stay
    };

    stateObject state;

    Player(sf::Image &image, float X, float Y, int W, int H, const sf::String &Name) : Entity(image, X, Y, W, H, Name)
    {
        state = stay;

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
            dy = -0.5;
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

    void update(float time)
    {
        control();

        switch (state)//тут делаются различные действия в зависимости от состояния
        {
            case right:
                dx = speed;
                break;//состояние идти вправо
            case left:
                dx = -speed;
                break;//состояние идти влево
            case up:
                break;//будет состояние поднятия наверх (например по лестнице)
            case down:
                break;//будет состояние во время спуска персонажа (например по лестнице)
            case jump:
                break;//здесь может быть вызов анимации
            case stay:
                break;//и здесь тоже
        }

        speed = 0;

        x += dx * time;
        checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х

        y += dy * time;
        checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y

        sprite.setPosition(x + w / 2, y + h / 2);

        if (health <= 0)
        {
            life = false;
        }

        if (life)
        {
            getPlayerCoordinateForView(x, y);
        }

        dy = dy + 0.0015 * time;//делаем притяжение к земле
    }

    void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
    {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
        {
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
                {
                    if (Dy > 0)
                    {
                        y = i * 32 - h;
                        dy = 0;
                        onGround = true;
                    }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать

                    if (Dy < 0)
                    {
                        y = i * 32 + 32;
                        dy = 0;
                    }//столкновение с верхними краями карты(может и не пригодиться)

                    if (Dx > 0)
                    {
                        x = j * 32 - w;
                    }//с правым краем карты

                    if (Dx < 0)
                    {
                        x = j * 32 + 32;
                    }// с левым краем карты
                }
                else
                {
                    onGround - false;
                }
            }
        }
    }

//    void interactionWithMap()
//    {
//        char const scoreMapSymbol = 's';
//        char const healthDownMapSymbol = 'f';
//        char const healthUpMapSymbol = 'h';
//
//        for (int i = y / 32; i < (y + h) / 32; i++)
//            for (int j = x / 32; j < (x + w) / 32; j++)
//            {
//                if (TileMap[i][j] == '0')
//                {
//                    if (dy > 0)
//                    {
//                        y = i * 32 - h;
//                    }
//                    if (dy < 0)
//                    {
//                        y = i * 32 + 32;
//                    }
//                    if (dx > 0)
//                    {
//                        x = j * 32 - w;
//                    }
//                    if (dx < 0)
//                    {
//                        x = j * 32 + 32;
//                    }
//                }
//
//                if (TileMap[i][j] == scoreMapSymbol)
//                {
//                    score++;
//                    TileMap[i][j] = ' ';
//                }
//
//                if (TileMap[i][j] == healthDownMapSymbol)
//                {
//                    health -= 50;
//                    TileMap[i][j] = ' ';
//                }
//
//                if (TileMap[i][j] == healthUpMapSymbol)
//                {
//                    health += 10;
//                    TileMap[i][j] = ' ';
//                }
//            }
//    }
};

class Enemy : public Entity
{
public:
    Enemy(sf::Image &image, float X, float Y, int W, int H, const sf::String& Name) : Entity(image, X, Y, W, H, Name)
    {
        if (name == "EasyEnemy")
        {
            sprite.setTextureRect(sf::IntRect(0, 0, w, h));
            dx = 0.1;//даем скорость.этот объект всегда двигается
        }
    }

    void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
    {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
        {
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
                {
                    if (Dy > 0)
                    {
                        y = i * 32 - h;
                    }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать

                    if (Dy < 0)
                    {
                        y = i * 32 + 32;
                    }//столкновение с верхними краями карты(может и не пригодиться)

                    if (Dx > 0)
                    {
                        x = j * 32 - w;
                        dx = -0.1;
                        sprite.scale(-1, 1);
                    }//с правым краем карты

                    if (Dx < 0)
                    {
                        x = j * 32 + 32;
                        dx = 0.1;
                        sprite.scale(-1, 1);
                    }// с левым краем карты
                }
            }
        }
    }

    void update(float time)
    {
        if (name == "EasyEnemy")
        {//для персонажа с таким именем логика будет такой
            //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
            checkCollisionWithMap(dx, dy);//обрабатываем столкновение по Х
            x += dx * time;
            sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра

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

    sf::Font font;
    font.loadFromFile("upload/font/EuclidCircularB-Regular.ttf");
    sf::Text healthText("", font, 20);
    sf::Text timeText("", font, 20);
    sf::Text scoreText("", font, 20);

    healthText.setColor(sf::Color::Red);
    timeText.setColor(sf::Color::Red);
    scoreText.setColor(sf::Color::Red);

    sf::Image map_image;
    map_image.loadFromFile("upload/images/map.png");
    sf::Texture map;
    map.loadFromImage(map_image);
    sf::Sprite s_map;
    s_map.setTexture(map);

    sf::Image heroImage;
    heroImage.loadFromFile("upload/images/hero.png");

    sf::Image easyEnemyImage;
    easyEnemyImage.loadFromFile("upload/images/cactus.png");

    Player dino(heroImage, 50, 750, 88, 94, "Player1");
    Enemy easyEnemy(easyEnemyImage, 850, 693, 55, 74, "EasyEnemy");

    sf::Clock clock;
    sf::Clock gameTimeClock;
    int gameTime = 0;

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

        gameTime = gameTimeClock.getElapsedTime().asSeconds();

        dino.update(time);
        easyEnemy.update(time);
        window.setView(view);
        window.clear();

        char const scoreMapSymbol = 's';
        char const healthDownMapSymbol = 'f';
        char const healthUpMapSymbol = 'h';
        char const mapBorder = '0';

        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ')
                {
                    s_map.setTextureRect(sf::IntRect(0, 0, 32, 32));
                }
                if (TileMap[i][j] == scoreMapSymbol)
                {
                    s_map.setTextureRect(sf::IntRect(32, 0, 32, 32));
                }
                if (TileMap[i][j] == mapBorder)
                {
                    s_map.setTextureRect(sf::IntRect(64, 0, 32, 32));
                }
                if (TileMap[i][j] == healthDownMapSymbol)
                {
                    s_map.setTextureRect(sf::IntRect(96, 0, 32, 32));
                }
                if (TileMap[i][j] == healthUpMapSymbol)
                {
                    s_map.setTextureRect(sf::IntRect(128, 0, 32, 32));
                }

                s_map.setPosition(j * 32, i * 32);

                window.draw(s_map);

                std::ostringstream gameHealthString, gameTimeString, gameScoreString;
                gameHealthString << dino.health;
//                gameScoreString << dino.score;
                gameTimeString << gameTime;
                healthText.setString("Health: " + gameHealthString.str());
                healthText.setPosition(view.getCenter().x + (float) CAM_WIDTH / 2 - 125, view.getCenter().y - (float) CAM_HEIGHT / 2 + 20);
                timeText.setString("Time: " + gameTimeString.str());
                timeText.setPosition(view.getCenter().x + (float) CAM_WIDTH / 2 - CAM_WIDTH + 25, view.getCenter().y - (float) CAM_HEIGHT / 2 + 20);
                scoreText.setString("Bitcoin: " + gameScoreString.str());
                scoreText.setPosition(view.getCenter().x + (float) CAM_WIDTH / 2 - 250, view.getCenter().y - (float) CAM_HEIGHT / 2 + 20);
            }

        window.draw(easyEnemy.sprite);
        window.draw(dino.sprite);
        window.draw(healthText);
        window.draw(timeText);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}