#include "Puppet.h"
#include "IPuppeteer.h"

USING_NS_CC;

bool CPuppet::init(IMapPhysics &mapPhysics)
{
    m_mapPhysics = &mapPhysics;
    return Node::init();
}

void CPuppet::onEnter()
{
    Node::onEnter();
    scheduleUpdate();
    if (m_puppeteer)
    {
        m_puppeteer->OnEnter();
    }
}

void CPuppet::onExit()
{
    unscheduleUpdate();
    if (m_puppeteer)
    {
        m_puppeteer->OnExit();
    }
    Node::onExit();
}

void CPuppet::update(float delta)
{
    if (m_puppeteer && m_mapPhysics)
    {
        m_puppeteer->Update(delta);
        auto maybeMove = [this](float x, float y)
        {
            const Vec2 oldPosition = getPosition();
            setPosition(oldPosition + Vec2(x, y));
            return true;
        };
        Vec2 offset = delta * m_puppeteer->GetVelocity();
        maybeMove(offset.x, offset.y) || maybeMove(offset.x, 0) || maybeMove(0, offset.y);
    }
}

Vec2 CPuppet::GetCenterInWorld() const
{
    Vec2 localCenter(0, 0);
    return convertToWorldSpace(localCenter);
}

void CPuppet::SetPuppeteer(IPuppeteer *puppeteer)
{
    if (m_puppeteer && isRunning())
    {
        m_puppeteer->OnExit();
    }
    m_puppeteer = puppeteer;
    if (m_puppeteer && isRunning())
    {
        m_puppeteer->OnEnter();
    }
}
