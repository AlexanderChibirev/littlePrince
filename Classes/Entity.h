#pragma once
#include "cocos2d.h"
#include "MotorSystem.h"


class CEntity 
	: public cocos2d::Sprite
	, public CMotorSystem
{
public: 
	CEntity();
	~CEntity();

	virtual void Update(float dt) = 0;
	virtual void Move(float dt);

	float GetHealthPoint();
	float GetDamage();
	void SetHealthPoint(float health);
	void SetDamage(float damage);
	bool IsLife();
	void Destroy();
private:
	float m_health;
	float m_damage;
	bool m_isLife;

};

