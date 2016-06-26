#pragma once
#ifndef __GAME_Scene_H__
#define __GAME_Scene_H__

#include "cocos2d.h"
#include "Block.h"
#include "SimpleAudioEngine.h"
#include "GlobalVar.h"
#include "StartScene.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	void setPhysicsWorld(PhysicsWorld * world);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init(PhysicsWorld* world);

	// implement the "static create()" method manually
	static GameScene* create(PhysicsWorld* world);


private:
	Sprite* player;
	Sprite* controlCenter;
	PhysicsWorld* m_world;
	Size visibleSize;
	cocos2d::Vec2 origin;
	std::vector<Sprite*> bullets;
	std::vector<Sprite*> blocks;
	float shootInterval;
	float creatInterval;
	float bulletSpeed;
	float time;

	cocos2d::Label* scoreLable;
	int score;

	cocos2d::Label* highestScoreLable;
	int highestScore;

	void addTouchListener();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void addContactListener();
	void addEdge();
	void addPlayer();
	bool onConcactBegan(PhysicsContact& contact);
	void fire();
	void createBlockS(float);
	void mergeBlock(Block* block1, Block* block2);
	void createBlockC(float);
	void update(float dt) override;
	void removeBlock(Sprite*);
	void updateScore(int score, cocos2d::Label*, int flag);
	void initBlockPhysicalBody(Block* block);
	void PlayBackgroundMusic();
	void Clear();
	void GameOver(Block* block);
};

#endif // __GAME_Scene_H__

