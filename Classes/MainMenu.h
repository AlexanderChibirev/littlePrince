#pragma once
#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MainMenu);
private:
	void OnMenuClose(cocos2d::Ref* pSender);
	void OnMenuPlay(cocos2d::Ref* pSender);
	void OnOptionsPlay(cocos2d::Ref* pSender);
	void GoToGameScene(cocos2d::Ref *sender);
};

