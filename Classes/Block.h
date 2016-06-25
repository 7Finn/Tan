#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#include "cocos2d.h"
using namespace cocos2d;

enum blockType {Square, Circle};

class Block : public CCSprite {
public:
	Block();
	static Block* createSquareBlock(int number, int width, int height); //���������η���
	static Block* createCircleBlock(int number, float radius); //����Բ��
	int getNumber();//��ȡ���������
	void setNumber(int number);//���÷��������
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
	CCSprite* content;
	void initSquareBlock(int number, int width, int height, Color3B color);
	void initCircleBlock(int number, float radius, Color3B color);
	void initLevelBlock();
};

#endif