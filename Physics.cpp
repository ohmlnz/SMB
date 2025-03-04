#include "Physics.h"
#include <iostream>

Vec2 Physics::getOverlap(const Vec2& entityOnePos, const Vec2& entityOneHalf, const Vec2& entityTwoPos, const Vec2& entityTwoHalf)
{
    Vec2 delta(abs(entityOnePos.x - entityTwoPos.x), abs(entityOnePos.y - entityTwoPos.y));
    Vec2 overlap((entityOneHalf.x + entityTwoHalf.x) - delta.x, (entityOneHalf.y + entityTwoHalf.y) - delta.y);

    return overlap;
}

bool Physics::isCollision(std::shared_ptr<Entity>& entityOne, std::shared_ptr<Entity>& entityTwo)
{
    const Vec2& entityOnePos = entityOne->getComponent<CTransform>().position;
    const Vec2& entityTwoPos = entityTwo->getComponent<CTransform>().position;

    const Vec2& entityOneHalf = entityOne->getComponent<CBoundingBox>().halfSize;
    const Vec2& entityTwoHalf = entityTwo->getComponent<CBoundingBox>().halfSize;

    auto overlap = getOverlap(entityOnePos, entityOneHalf, entityTwoPos, entityTwoHalf);

    if (overlap.x > 0 && overlap.y > 0)
    {
        entityOne->getComponent<CTransform>().overlap = overlap;
        return true;
    }

    return false;
}

short int Physics::resolveOverlap(std::shared_ptr<Entity>& entityOne, std::shared_ptr<Entity>& entityTwo)
{
    const Vec2& entityOnePrevPos = entityOne->getComponent<CTransform>().previousPosition;
    const Vec2& entityTwoPos = entityTwo->getComponent<CTransform>().position;

    const Vec2& entityOneHalf = entityOne->getComponent<CBoundingBox>().halfSize;
    const Vec2& entityTwoHalf = entityTwo->getComponent<CBoundingBox>().halfSize;

    auto previousOverlap = getOverlap(entityOnePrevPos, entityOneHalf, entityTwoPos, entityTwoHalf);
    auto currentOverlap = entityOne->getComponent<CTransform>().overlap;

    Vec2& entityOnePosition = entityOne->getComponent<CTransform>().position;
    
    // 0. LEFT - 1. RIGHT - 2. TOP - 3. BOTTOM

    if (previousOverlap.y > previousOverlap.x)
    {
        if (entityOnePrevPos.x < entityTwoPos.x)
        {
            entityOnePosition.x -= currentOverlap.x;
            return 0;
        }
        else
        {
            entityOnePosition.x += currentOverlap.x;
            return 1;
        }
    }
    else
    {
        if (entityOnePrevPos.y < entityTwoPos.y)
        {
            entityOnePosition.y -= currentOverlap.y;
            return 2;
        }
        else
        {
            entityOnePosition.y += currentOverlap.y;
            return 3;
        }
    }
}
