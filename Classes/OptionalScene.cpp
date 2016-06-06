#include "OptionalScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* OptionalScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OptionalScene::create();
    scene->addChild(layer);
    return scene;
}

bool OptionalScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto backgroundSprite = Sprite::create("forMenuScene/bgOption.png");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite);
	
	auto playItem = MenuItemImage::create("forMenuScene/BackToMenu.png", "forMenuScene/BackToMenuCliked.png", CC_CALLBACK_1(OptionalScene::goToMainMenuScene, this));
	playItem->setPosition(Point(-380, visibleSize.height / 2 - 40));
	auto menuPlay = Menu::create(playItem, NULL);	
	this->addChild(menuPlay);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/option.mp3", true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
	return true;
}

void OptionalScene::goToMainMenuScene(cocos2d::Ref *sender)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(3.0f, scene));
}