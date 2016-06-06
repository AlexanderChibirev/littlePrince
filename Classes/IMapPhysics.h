#pragma once

#include "cocos_custom.h"

class IMapPhysics
{
public:
    virtual ~IMapPhysics() = default;

    /// @param from - point in world coordinates
    /// @param to - point in world coordinates
    virtual bool CanWalkDirectly(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to)const = 0;
    /// @param worldPosition - point in world coordinates
    virtual bool CanStandOn(const cocos2d::Vec2 &worldPosition) const = 0;
};
