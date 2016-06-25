#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#include "cocos2d.h"
using namespace cocos2d;

enum blockType {Square, Circle};

class Block : public CCSprite {
public:
	Block();
	static Block* createSquareBlock(int number, int width, int height); //创建正方形方块
	static Block* createCircleBlock(int number, float radius); //创建圆形
	int getNumber();//获取方块的数字
	void setNumber(int number);//设置方块的数字
	bool getIsMerging() { return this->isMerging; };
	void setIsMerging(bool merging) { this->isMerging = merging; };
	void setType(blockType type) { this->type = type; }
	blockType getType() { return type; }
private:
	int b_number;
	CCLabelTTF* b_lablenumber;
	CCLayerColor* b_layercolor;
	bool isMerging;
	blockType type;
	void initSquareBlock(int number, int width, int height);
	void initCircleBlock(int number, float radius);
};

#endif