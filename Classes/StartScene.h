#pragma once
#ifndef __MENU_SEBCE_H__
#define __MENU_SEBCE_H__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class StartScene : public cocos2d::Layer
{
public:
	void setPhysicsWorld(PhysicsWorld * world);

	static cocos2d::Scene* createScene();

	virtual bool init(PhysicsWorld* world);

	static StartScene* create(PhysicsWorld* world);

	void ToGameSence(cocos2d::Ref* pSender);
	// implement the "static create()" method manually

private:
	PhysicsWorld* m_world;
	Size visibleSize;
	Vec2 origin;


	void AddEdge();
	void AddStartBtn();
	void ShootBullet();
};

#endif // __MENU_SEBCE_H__
