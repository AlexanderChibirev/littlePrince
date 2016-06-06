#pragma once

#include "cocos_custom.h"
#include "IMapPhysics.h"

class IPuppeteer;

class CPuppet : public cocos2d::Node
{
public:
    bool init(IMapPhysics & mapPhysics);

    void onEnter() override;
    void onExit() override;
    void update(float delta) override;

    cocos2d::Vec2 GetCenterInWorld()const;
    void SetPuppeteer(IPuppeteer *puppeteer);

private:
    IPuppeteer *m_puppeteer = nullptr;
    IMapPhysics *m_mapPhysics = nullptr;
};

typedef cocos2d::RefPtr<CPuppet> CPuppetPtr;
