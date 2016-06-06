#pragma once
#include "cocos2d.h" 
#include "Joystick.h"
#include "Entity.h"
#include <vector>
#include <map>

class Player : public CEntity
{
public:
	static Player *create(Joystick *moveJoy/*, Joystick *rotateJoy Legs * legs*/);
	void Update(float dt) override;
	void Shoot(cocos2d::Layer *layer);
	//void ChooseNextGun();
	void Stay();
	void Jump();
	Vec2 GetPositionHero();
	void SetGravityHero(float gravity);
	bool m_onGround = false;
	bool attak = false;
	void SetHPLine(cocos2d::Sprite * line);
	void UpdateHPLine();
	cocos2d::RefPtr<cocos2d::Animate> m_enemyAttack;
	bool m_jumpH = false;
	void Fall();
	void  SetHP(float hp);
	
private:
	Player();
	enum state { stay, run, gun, jump, left, right, fall } m_state;
	void initOptions();
	//void SetState(double const& move);

	void initAnimations();

	Joystick * m_moveController;
	enum CurrentGun { FireGun, Pistol, runs } m_gun;

	std::map<CurrentGun, std::vector<cocos2d::RefPtr<Animate>>> m_states;
	std::vector<cocos2d::RefPtr<Animate>> m_currentState;

	cocos2d::RefPtr<Animate> stayWithKnife;
	cocos2d::RefPtr<Animate> runWithKnife;
	cocos2d::RefPtr<Animate> m_currentAction;
	MovementDirection m_shootDirection;
	bool m_isShooting;
	void Left();
	void Right();
	bool m_fallBegin = false;
	float m_timeSinceLastUpdate = 0;
	cocos2d::RefPtr<cocos2d::Animate> m_run;
	cocos2d::RefPtr<cocos2d::Animate> m_left;
	cocos2d::RefPtr<cocos2d::Animate> m_right;
	cocos2d::RefPtr<cocos2d::Animate> m_fall;
	cocos2d::RefPtr<cocos2d::Animate> m_jump;
	float m_dx = 0;
	float m_dy = 0;
	float m_velocityVec = 0;
	float m_timeAttak = 0;
	float m_timeFall = 0;
	cocos2d::Sprite * m_healthLine;
};

