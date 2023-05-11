#pragma once
#include "Entity.h"

class Physics
{
private:
    Vec2 m_previousOverlap  = { 0, 0 };
    Vec2 m_currentOverlap   = { 0, 0 };
public:

    Vec2 getOverlap(const Vec2& entityOnePos, const Vec2& entityOneHalf, const Vec2& entityTwoPos, const Vec2& entityTwoHalf);
    // TODO: rename collidee/collider
    bool isCollision(std::shared_ptr<Entity>& entityOne, std::shared_ptr<Entity>& entityTwo);
    short int resolveOverlap(std::shared_ptr<Entity>& entityOne, std::shared_ptr<Entity>& entityTwo);
};