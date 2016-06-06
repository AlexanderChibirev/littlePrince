#pragma once

#include "cocos_custom.h"

class IPuppeteer
{
public:
    virtual ~IPuppeteer() = default;

    virtual void OnEnter() = 0;
    virtual void Update(float delta) = 0;
    virtual void OnExit() = 0;

    virtual cocos2d::Vec2 GetVelocity()const = 0;
};
