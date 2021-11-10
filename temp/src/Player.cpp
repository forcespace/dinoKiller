////
//// Created by Сергей Павлов on 24.10.2021.
////
//
//#include "Player.h"
//
//Player::Player(const sf::String &F, float X, float Y, float W, float H)
//    : dx(0), dy(0), w(W), h(H), x(X), y(Y), speed(0)
//{
//    File = F;
//    image.loadFromFile("upload/images/" + File);
//    image.createMaskFromColor(sf::Color(41, 33, 59));
//    texture.loadFromImage(image);
//    sprite.setTexture(texture);
//    sprite.setTextureRect(sf::IntRect(0, 0, (int) w, (int) h));
//}
//
//void Player::update(float time)
//{
//    switch (dir)
//    {
//        case 0:
//            dx = speed;
//            dy = 0;
//            break;
//        case 1:
//            dx = -speed;
//            dy = 0;
//            break;
//        case 2:
//            dx = 0;
//            dy = speed;
//            break;
//        case dirUp:
//            dx = 0;
//            dy = -speed;
//            break;
//    }
//
//    x += dx * time;
//    y += dy * time;
//
//    speed = 0;
//    sprite.setPosition(x, y);
//}