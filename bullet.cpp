#include "bullet.h"

Bullet::Bullet(sf::Vector2f initialPosition, double angle)
{
    this->position = initialPosition;
    this->angel = angle;
}

void Bullet::setSpeed(double speed){
    this->speed = speed;
}

void Bullet::setSender(int sender){
    this->sender = sender;
}

void Bullet::update(float time){
    this->position.x += speed * time * cos(angel);
    this->position.y += speed * time * sin(angel);
}

sf::FloatRect Bullet::getBoundingBox(){
    return sf::FloatRect(position.x, position.y, 4.f, 4.f);
}

int Bullet::getSender(){
    return sender;
}
