#pragma once
//#include "cocos_custom.h"
#include "IMapPhysics.h"


/// Data model for my own custom maps.
class CCustomMap
        : public cocos2d::TMXTiledMap
        , public IMapPhysics
{
public:
    /// @param from - point in world coordinates
    /// @param to - point in world coordinates
    bool CanWalkDirectly(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to)const override;
    /// @param worldPosition - point in world coordinates
    bool CanStandOn(const cocos2d::Vec2 &worldPosition)const override;
	
	cocos2d::Rect GetHeroWorldPosition()const;
	std::vector<cocos2d::Rect> GetObcaslePosition()const;
	std::vector<cocos2d::Rect> GetObcasleBonusJumpPosition()const;
	std::vector<cocos2d::Rect> GetObcasleBonusHPPosition() const;
    std::vector<cocos2d::Vec2> GetEnemyWorldPositions()const;
	std::vector<cocos2d::Rect> GetObcasleOnGround()const;
    bool init(const std::string& tmxFile);
	std::vector<cocos2d::Rect> CCustomMap::GetObcasleCoogwheel() const;
	std::vector<cocos2d::Rect> CCustomMap::GetObcasleKey() const;
private:
    bool InitPassabilityMap();
    bool LoadObstacles();
	bool LoadOnGround();
    bool LoadUnits();
	bool LoadBonusJump();
	bool LoadBonusHP();
	bool LoadObstaclesKey();
	bool LoadCoogwheel();
    cocos2d::Rect AsRect(const cocos2d::ValueMap &properties)const;

    std::vector<bool> m_passability;
    std::vector<cocos2d::Rect> m_obstacles;
	std::vector<cocos2d::Rect> m_obstaclesCoogwheel;
	std::vector<cocos2d::Rect> m_obstaclesKey;
	std::vector<cocos2d::Rect> m_obstaclesForEnemy;
    std::vector<cocos2d::Vec2> m_enemyPositions;
	std::vector<cocos2d::Rect> m_obstaclesOnGround;
	std::vector<cocos2d::Rect> m_obstaclesBounusJump;
	std::vector<cocos2d::Rect> m_obstaclesBounusHP;
    cocos2d::Rect m_heroPosition;

};
