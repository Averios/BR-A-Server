#ifndef BULLET_H
#define BULLET_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class Bullet
{
public:
    Bullet(sf::Vector2f initialPosition, double angle);
    void setSender(int sender);
    void setSpeed(double speed);
    void update(float time);
    int getSender();
    sf::FloatRect getBoundingBox();
private:
    sf::Vector2f position;
    double angel;
    int sender;
    double speed;

};

#endif // BULLET_H
