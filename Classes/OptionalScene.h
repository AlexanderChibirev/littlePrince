#pragma once
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "MainMenu.h"


class OptionalScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(OptionalScene);
private:
	void goToMainMenuScene(cocos2d::Ref *sender);
};
