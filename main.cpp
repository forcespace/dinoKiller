#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "src/map.h"
#include "src/view.h"
#include "src/level.h"
#include <vector>
#include <list>

constexpr unsigned WINDOW_WIDTH = 1920;
constexpr unsigned WINDOW_HEIGHT = 1080;
constexpr unsigned CAM_WIDTH = WINDOW_WIDTH;
constexpr unsigned CAM_HEIGHT = WINDOW_HEIGHT;

class Entity
{
public:
    std::vector<Object> obj;//вектор объектов карты
    float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
    int w, h, health;
    bool life, onGround;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::String name;//враги могут быть разные, мы не будем делать другой класс для врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени

    Entity(sf::Image &image, const sf::String &Name, float X, float Y, int W, int H)
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

    sf::FloatRect getRect()
    {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
        return sf::FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений
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
            dy = -0.75;
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
        for (int i = 0; i < obj.size(); i++)//проходимся по объектам
        {
            if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
            {
                if (obj[i].name == "solid")//если встретили препятствие
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
};

class Enemy : public Entity
{
public:
    Enemy(sf::Image &image, const sf::String &Name, Level &lvl, float X, float Y, int W, int H) : Entity(image, Name, X, Y, W, H)
    {
        obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой

        if (name == "easyEnemy")
        {
            sprite.setTextureRect(sf::IntRect(0, 0, w, h));
            dx = 0.1;//даем скорость.этот объект всегда двигается
        }
    }

    void checkCollisionWithMap(float Dx, float Dy)
    {
        for (int i = 0; i < obj.size(); i++)//проходимся по объектам
        {
            if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
            {
                //if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
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
                //}
            }
        }
    }

    void update(float time)
    {
        if (name == "easyEnemy")
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

    Level lvl;//создали экземпляр класса уровень
    lvl.LoadFromFile("src/map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

    sf::Font font;
    font.loadFromFile("upload/font/EuclidCircularB-Regular.ttf");
    sf::Text healthText("", font, 20);
    sf::Text timeText("", font, 20);
    sf::Text scoreText("", font, 20);

    healthText.setColor(sf::Color::Red);
    timeText.setColor(sf::Color::Red);
    scoreText.setColor(sf::Color::Red);

    sf::Image heroImage;
    heroImage.loadFromFile("upload/images/hero.png");

    sf::Image easyEnemyImage;
    easyEnemyImage.loadFromFile("upload/images/cactus.png");

    std::list<Entity *> entities;//создаю список, сюда буду кидать объекты.например врагов.
    std::list<Entity *>::iterator it;//итератор чтобы проходить по эл-там списка
    std::list<Entity *>::iterator it2;//второй итератор.для взаимодействия между объектами списка

    std::vector<Object> e = lvl.GetObjects("easyEnemy");//все объекты врага на tmx карте хранятся в этом векторе

    for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
    {
        entities.push_back(new Enemy(easyEnemyImage, "easyEnemy", lvl, e[i].rect.left, e[i].rect.top, 55, 74));//и закидываем в список всех наших врагов с карты
    }

    Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
//    Object easyEnemyObject = lvl.GetObject("easyEnemy");//объект легкого врага на нашей карте.задаем координаты игроку в начале при помощи него

    Player dino(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 88, 94);//передаем координаты прямоугольника player из карты в координаты нашего игрока
//    Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 55,74);//передаем координаты прямоугольника easyEnemy из карты в координаты нашего врага

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

        dino.update(time);

        for (it = entities.begin(); it != entities.end(); it++)
        {
            (*it)->update(time);
        }//для всех элементов списка(пока это только враги,но могут быть и пули к примеру) активируем ф-цию update

        //easyEnemy.update(time);//старый вариант update врага

        for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
        {
            if ((*it)->getRect().intersects(dino.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
            {
                if ((*it)->name == "easyEnemy")
                {//и при этом имя объекта EasyEnemy,то..
                    if ((*it)->dx > 0)//если враг идет вправо
                    {
                        (*it)->x = dino.x - (*it)->w; //отталкиваем его от игрока влево (впритык)
                        (*it)->dx = 0;//останавливаем
                    }

                    if ((*it)->dx < 0)//если враг идет влево
                    {
                        (*it)->x = dino.x + dino.w; //аналогично - отталкиваем вправо
                        (*it)->dx = 0;//останавливаем
                    }
                }
            }

            for (it2 = entities.begin(); it2 != entities.end(); it2++)
            {
                if ((*it)->getRect() != (*it2)->getRect())
                {//при этом это должны быть разные прямоугольники
                    if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "easyEnemy") && ((*it2)->name == "easyEnemy"))//если столкнулись два объекта и они враги
                    {
                        (*it)->dx *= -1;//меняем направление движения врага
                        (*it)->sprite.scale(-1, 1);//отражаем спрайт по горизонтали
                    }
                }
            }
        }

        window.setView(view);
        window.clear();
        lvl.Draw(window);//рисуем новую карту

        for (it = entities.begin(); it != entities.end(); it++)
        {
            window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
        }

//        window.draw(easyEnemy.sprite);
        window.draw(dino.sprite);
        window.display();
    }

    return 0;
}